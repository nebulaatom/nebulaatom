/*
 * CPW Woodpecker Server
 * Copyright (C) 2021 CPW Online support@cpwonline.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tools/file_manager.h"

using namespace CPW::Tools;

FileManager::FileManager() :
	operation_type_(OperationType::kDownload)
	,directory_base_("/srv/www")
	,directory_for_uploaded_files_("/srv/www/uploaded-files")
	,directory_for_temp_files_("/tmp")
{
	AddSupportedFiles_();
	result_ = new JSON::Array();
}

FileManager::~FileManager()
{

}

void FileManager::handlePart(const MessageHeader& header, std::istream& stream)
{
    std::string filename, name;

	std::string content_type = header.get("Content-Type", "(unspecified)");
	if (header.has("Content-Disposition"))
	{
		std::string disp;
		NameValueCollection params;
		MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
		name = params.get("name", "(unnamed)");
		filename = params.get("filename", "(unnamed)");
	}

	CountingInputStream istr(stream);
	std::ofstream ostr;
	ostr.open(directory_for_temp_files_ + "/" + filename);
	StreamCopier::copyStream(istr, ostr);
	std::size_t content_length = istr.chars();
	ostr.close();

	bool check = false;
    Extras::File current_file(name, filename, content_type, content_length);
    std::shared_ptr<Path> tmp_path;
	do
	{
		tmp_path.reset(new Path(GenerateName_(filename)));
        current_file.get_requested_path() = tmp_path;

		check = CheckFile_(current_file);
	}
	while(!check);

    current_file.get_requested_file().reset(new File(*current_file.get_requested_path()));

    files_.push_back(current_file);
}

std::string FileManager::GenerateName_(std::string name)
{
	DateTime now_time;
	Random random_number;

	random_number.seed();
	std::string new_name =
		std::to_string(now_time.year())
		+ std::to_string(now_time.month())
		+ std::to_string(now_time.day())
		+ "_"
		+ std::to_string(now_time.hour())
		+ std::to_string(now_time.minute())
		+ std::to_string(now_time.second())
		+ std::to_string(now_time.millisecond())
		+ "_"
		+ std::to_string(random_number.next())
		+ "_"
		+ name
	;
	return new_name;
}

bool FileManager::CheckFile_(Path path)
{
	switch(operation_type_)
	{
		case OperationType::kDownload:
		case OperationType::kReplace:
		case OperationType::kDelete:
		{
			requested_path_ = std::make_shared<Path>(directory_base_, Path::PATH_NATIVE);
			requested_path_->append(path);
			requested_file_ = std::make_shared<File>(*requested_path_);

			if(requested_file_->exists())
			{
				if(requested_file_->isDirectory())
				{
					requested_path_->makeDirectory();
					requested_path_->setFileName("index.html");

					requested_file_.reset(new File(*requested_path_));

					if(!requested_file_->exists())
						return false;
				}
			}
			else
				return false;

			if(!requested_file_->canRead())
				return false;
			else
				return true;
			break;
		}
		case OperationType::kUpload:
		{
			requested_path_ = std::make_shared<Path>(directory_for_uploaded_files_, Path::PATH_NATIVE);
			requested_path_->append(path);
			requested_file_ = std::make_shared<File>(*requested_path_);

			if(requested_file_->exists())
				return false;
			else
				return true;
		}
	}
}

bool FileManager::IsSupported_()
{
	std::string extension = requested_path_->getExtension();
	auto file_found = supported_files_.find(extension);
	if(file_found != supported_files_.end())
	{
		file_properties_ = &file_found->second;
		return true;
	}
	else
	{
		for(auto it : supported_files_)
		{
			auto extensions = it.second.get_other_extensions();
			auto find_depth = std::find(extensions.begin(), extensions.end(), extension);
			if(find_depth != extensions.end())
			{
				file_properties_ = &it.second;
				return true;
			}
		}
		return false;
	}
}

void FileManager::DownloadFile_(std::ostream& out_response)
{
	switch(file_type_)
	{
		case FileType::kBinary:
		{
			std::ifstream requested_file(requested_path_->toString(), std::ios::binary | std::ios::ate);

			content_length_ = requested_file.tellg();
			std::string text_line(content_length_, '\0');
			requested_file.seekg(0);
			if(requested_file.read(&text_line[0], content_length_))
			{
				out_response << text_line;
			}
			requested_file.close();
			break;
		}
		case FileType::kTextPlain:
		{
			std::string text_line;
			std::ifstream requested_file(requested_path_->toString());
			while (getline (requested_file, text_line))
			{
				out_response << text_line << "\n";
			}
			requested_file.close();
			break;
		}
	}
}

void FileManager::UploadFile_()
{
	JSON::Object::Ptr object = new JSON::Object();

	if (!name_.empty())
	{
		std::unique_ptr<File> tmp_file = std::make_unique<File>(Path(directory_for_temp_files_ + "/" + filename_));
		tmp_file->moveTo(requested_file_->path());

		object->set("name", name_);
		object->set("filename", requested_path_->getFileName());
		object->set("type" ,content_type_);
		object->set("size", content_length_);

		result_->set(result_->size(), object);
	}
}

void FileManager::RemoveFile_()
{

}

void FileManager::ProcessFileType_()
{
	if(IsSupported_())
	{
		bool file_is_binary = file_properties_->get_binary();

		if(file_is_binary)
			file_type_ = FileType::kBinary;
		else
			file_type_ = FileType::kTextPlain;
	}
	else
		file_type_ = FileType::kBinary;
}

void FileManager::AddSupportedFiles_()
{
	supported_files_.emplace(std::make_pair("html", Extras::FileProperties{"text/html", false, {"htm", "html5"}}));
	supported_files_.emplace(std::make_pair("js", Extras::FileProperties{"text/javascript", false, {"js5"}}));
	supported_files_.emplace(std::make_pair("css",Extras::FileProperties{"text/css", false, {"css3"}}));
	supported_files_.emplace(std::make_pair("jpeg",Extras::FileProperties{"image/jpeg", false, {"jpeg", "jpg"}}));
	supported_files_.emplace(std::make_pair("png",Extras::FileProperties{"image/png", true, {""}}));
	supported_files_.emplace(std::make_pair("svg",Extras::FileProperties{"image/svg+xml", true, {""}}));
	supported_files_.emplace(std::make_pair("ttf",Extras::FileProperties{"font/ttf", true, {""}}));
	supported_files_.emplace(std::make_pair("woff",Extras::FileProperties{"font/woff", true, {""}}));
	supported_files_.emplace(std::make_pair("woff2",Extras::FileProperties{"font/woff2", true, {""}}));
	supported_files_.emplace(std::make_pair("xhtml",Extras::FileProperties{"application/xhtml+xml", true, {""}}));
	supported_files_.emplace(std::make_pair("webm",Extras::FileProperties{"video/webm", true, {""}}));
	supported_files_.emplace(std::make_pair("xml",Extras::FileProperties{"application/xml", true, {""}}));
	supported_files_.emplace(std::make_pair("zip",Extras::FileProperties{"application/zip", true, {""}}));
	supported_files_.emplace(std::make_pair("wav",Extras::FileProperties{"audio/x-wav", true, {""}}));
	supported_files_.emplace(std::make_pair("pdf",Extras::FileProperties{"application/pdf", true, {""}}));
	supported_files_.emplace(std::make_pair("mpeg",Extras::FileProperties{"video/mpeg", true, {""}}));
	supported_files_.emplace(std::make_pair("json",Extras::FileProperties{"application/json", true, {""}}));
	supported_files_.emplace(std::make_pair("ico",Extras::FileProperties{"image/x-icon", true, {""}}));
	supported_files_.emplace(std::make_pair("gif",Extras::FileProperties{"image/gif", true, {""}}));
	supported_files_.emplace(std::make_pair("avi",Extras::FileProperties{"video/x-msvideo", true, {""}}));
	supported_files_.emplace(std::make_pair("txt",Extras::FileProperties{"text/plain", true, {""}}));
}
