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
    Extras::File tmp_file;
    Extras::File current_file;
    bool check = false;

    // Get header parameters
        current_file.set_content_type(header.get("Content-Type", "(unspecified)"));
        current_file.set_name(SplitHeaderValue_(header, "Content-Disposition", "name"));
        current_file.set_filename(SplitHeaderValue_(header, "Content-Disposition", "filename"));
        tmp_file.set_filename(SplitHeaderValue_(header, "Content-Disposition", "filename"));

    // Check temporary file
        do
        {
            tmp_file.get_requested_path().reset
            (
                new Path(GenerateName_(directory_for_temp_files_ + "/" + tmp_file.get_filename()))
            );

            check = CheckFile_(tmp_file);
        }
        while(!check);

    // Create temporary file
        CountingInputStream istr(stream);
        std::ofstream ostr;
        ostr.open(tmp_file.get_requested_path()->toString());
        StreamCopier::copyStream(istr, ostr);
        ostr.close();

    // Check final target file
        check = false;
        do
        {
            current_file.get_requested_path().reset
            (
                new Path(GenerateName_(current_file.get_filename()))
            );

            check = CheckFile_(current_file);
        }
        while(!check);

    // Add new file
        current_file.get_requested_file().reset(new File(*current_file.get_requested_path()));
        current_file.get_tmp_file().reset(new File(*tmp_file.get_requested_path()));
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

bool FileManager::CheckFile_(Extras::File& current_file)
{
    auto& requested_path = current_file.get_requested_path();
    auto& requested_file = current_file.get_requested_file();

    switch(operation_type_)
    {
        case OperationType::kDownload:
        case OperationType::kDelete:
        {
            auto filename = requested_path->toString();
            requested_path.reset(new Path(directory_base_, Path::PATH_NATIVE));
            requested_path->append(Path(filename));
            requested_file.reset(new File(*requested_path));

            if(requested_file->exists())
            {
                if(requested_file->isDirectory())
                {
                    requested_path->makeDirectory();
                    requested_path->setFileName("index.html");

                    requested_file.reset(new File(*requested_path));

                    if(!requested_file->exists())
                        return false;
                }
            }
            else
                return false;

            if(!requested_file->canRead())
                return false;

            break;
        }
        case OperationType::kUpload:
        {
            auto filename = requested_path->getFileName();
            requested_path.reset(new Path(directory_for_uploaded_files_, Path::PATH_NATIVE));
            requested_path->append(Path(filename));
            requested_file.reset(new File(*requested_path));

            if(requested_file->exists())
                return false;
        }
    }

    return true;
}

bool FileManager::CheckFiles_()
{
    for(auto& file_it : files_)
    {
        if(!CheckFile_(file_it))
            return false;
    }

    return true;
}

bool FileManager::IsSupported_(Extras::File& file)
{
    if(file.get_requested_path() == nullptr)
        return false;

    auto basic_operations = [&file](Extras::FileProperties& properties)
    {
        file.set_file_properties(properties);
        file.set_content_type(properties.get_content_type());
    };

    std::string extension = file.get_requested_path()->getExtension();
    auto file_found = supported_files_.find(extension);
    if(file_found != supported_files_.end())
    {
        basic_operations(file_found->second);
        return true;
    }
    else
    {
        for(auto& it : supported_files_)
        {
            auto extensions = it.second.get_other_extensions();
            auto find_depth = std::find(extensions.begin(), extensions.end(), extension);
            if(find_depth != extensions.end())
            {
                basic_operations(it.second);
                return true;
            }
        }
        return false;
    }
}

void FileManager::ProcessContentLength_(Extras::File& file)
{
    file.set_content_length(file.get_requested_file()->getSize());
}

	switch(files_.front().get_file_type())
	{
        case Extras::FileType::kBinary:
		{
			std::ifstream requested_file(files_.front().get_requested_path()->toString(), std::ios::binary | std::ios::ate);

			content_length = requested_file.tellg();
			std::string text_line(content_length, '\0');
			requested_file.seekg(0);

			if(requested_file.read(&text_line[0], content_length))
			{
				out_response << text_line;
			}
			requested_file.close();
			break;
		}
		case Extras::FileType::kTextPlain:
		{
			std::string text_line;
			std::ifstream requested_file(files_.front().get_requested_path()->toString());
			content_length = requested_file.tellg();

			while (getline (requested_file, text_line))
			{
				out_response << text_line << "\n";
			}
			requested_file.close();
			break;
		}
	}

	files_.front().set_content_length(content_length);
}

void FileManager::UploadFile_()
{
    for(auto& file_it : files_)
    {
        JSON::Object::Ptr object = new JSON::Object();

        if (!file_it.get_name().empty())
        {
            std::string target = directory_for_temp_files_ + "/" + file_it.get_filename();
            std::string destiny = file_it.get_requested_file()->path();

            auto tmp_file = File(Path(target));
            tmp_file.moveTo(destiny);

            object->set("name", file_it.get_name());
            object->set("filename", file_it.get_requested_path()->getFileName());
            object->set("type", file_it.get_content_type());
            object->set("size", file_it.get_content_length());

            result_->set(result_->size(), object);
        }
    }
}

void FileManager::RemoveFile_()
{
    for(auto& file_it : files_)
    {
        file_it.get_requested_file()->remove();

        JSON::Object::Ptr object = new JSON::Object();
        object->set("file", file_it.get_filename());
        result_->set(result_->size(), object);
    }
}

void FileManager::ProcessFileType_()
{
    for(auto& file_it : files_)
    {
        if(IsSupported_(file_it))
        {
            bool file_is_binary = file_it.get_file_properties()->get_binary();

            if(file_is_binary)
                file_it.set_file_type(Extras::FileType::kBinary);
            else
                file_it.set_file_type(Extras::FileType::kTextPlain);
        }
        else
            file_it.set_file_type(Extras::FileType::kBinary);
    }
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
