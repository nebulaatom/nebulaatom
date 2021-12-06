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

#include "Factory/frontend_handler.h"

using namespace CPW::Factory;

FileProperties::FileProperties(std::string content_type, bool binary, std::vector<std::string> other_extensions) :
	content_type_(content_type)
	,binary_(binary)
	,other_extensions_(other_extensions)
{

}

FileProperties::~FileProperties()
{

}

FrontendHandler::FrontendHandler(std::string api_version) :
	RootHandler(api_version)
	,directory_base_("/srv/www")
{
	AddSupportedFiles_();
	requested_path_ = new Path(directory_base_, Path::PATH_NATIVE);
}

FrontendHandler::~FrontendHandler()
{
	delete requested_path_;
}

void FrontendHandler::AddRoutes_()
{
	get_routes_list().push_back(new Route("",std::vector<std::string>{""}));
}

void FrontendHandler::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	if(!CheckFile_(Path(request.getURI())))
	{
		GenericResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Requested file bad check.");
		return;
	}

	ManageFile_(response);
}

void FrontendHandler::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "This method is not supported.");
	return;
}

void FrontendHandler::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "This method is not supported.");
	return;
}

void FrontendHandler::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "This method is not supported.");
	return;
}

void FrontendHandler::AddSupportedFiles_()
{
	supported_files_.emplace(std::make_pair("html", FileProperties{"text/html", false, {"htm", "html5"}}));
	supported_files_.emplace(std::make_pair("js", FileProperties{"text/javascript", false, {"js5"}}));
	supported_files_.emplace(std::make_pair("css",FileProperties{"text/css", false, {"css3"}}));
	supported_files_.emplace(std::make_pair("jpeg",FileProperties{"image/jpeg", false, {"jpeg", "jpg"}}));
	supported_files_.emplace(std::make_pair("png",FileProperties{"image/png", true, {""}}));
	supported_files_.emplace(std::make_pair("svg",FileProperties{"image/svg+xml", true, {""}}));
	supported_files_.emplace(std::make_pair("ttf",FileProperties{"font/ttf", true, {""}}));
	supported_files_.emplace(std::make_pair("woff",FileProperties{"font/woff", true, {""}}));
	supported_files_.emplace(std::make_pair("woff2",FileProperties{"font/woff2", true, {""}}));
	supported_files_.emplace(std::make_pair("xhtml",FileProperties{"application/xhtml+xml", true, {""}}));
	supported_files_.emplace(std::make_pair("webm",FileProperties{"video/webm", true, {""}}));
	supported_files_.emplace(std::make_pair("xml",FileProperties{"application/xml", true, {""}}));
	supported_files_.emplace(std::make_pair("zip",FileProperties{"application/zip", true, {""}}));
	supported_files_.emplace(std::make_pair("wav",FileProperties{"audio/x-wav", true, {""}}));
	supported_files_.emplace(std::make_pair("pdf",FileProperties{"application/pdf", true, {""}}));
	supported_files_.emplace(std::make_pair("mpeg",FileProperties{"video/mpeg", true, {""}}));
	supported_files_.emplace(std::make_pair("json",FileProperties{"application/json", true, {""}}));
	supported_files_.emplace(std::make_pair("ico",FileProperties{"image/x-icon", true, {""}}));
	supported_files_.emplace(std::make_pair("gif",FileProperties{"image/gif", true, {""}}));
	supported_files_.emplace(std::make_pair("avi",FileProperties{"video/x-msvideo", true, {""}}));
	supported_files_.emplace(std::make_pair("txt",FileProperties{"text/plain", true, {""}}));
}

bool FrontendHandler::IsSupported_()
{
	std::string extension = requested_path_->getExtension();
	auto file_found = supported_files_.find(extension);
	if(file_found != supported_files_.end())
	{
		return true;
	}
	else
	{
		for(auto it : supported_files_)
		{
			auto extensions = it.second.get_other_extensions();
			auto find_depth = std::find(extensions.begin(), extensions.end(), extension);
			if(find_depth != extensions.end())
				return true;
		}
		return false;
	}
}

bool FrontendHandler::CheckFile_(Path path)
{
	requested_path_->append(path);
	std::unique_ptr<File> tmp_file (new File(*requested_path_));

	if(tmp_file->exists())
	{
		if(tmp_file->isDirectory())
		{
			requested_path_->makeDirectory();
			requested_path_->setFileName("index.html");

			tmp_file.reset(new File(*requested_path_));

			if(!tmp_file->exists())
				return false;
		}
	}
	else
		return false;

	if(!tmp_file->canRead())
		return false;
	else
		return true;
}

void FrontendHandler::ManageFile_(HTTPServerResponse& response)
{
	if(IsSupported_())
	{
		bool file_is_binary = supported_files_.at(requested_path_->getExtension()).get_binary();
		if(file_is_binary)
			ManageBinaryFile_(response);
		else
			ManageTextPlainFile_(response);
	}
	else
		ManageBinaryFile_(response);
}

void FrontendHandler::ManageBinaryFile_(HTTPServerResponse& response)
{
	response.setStatus(HTTPResponse::HTTP_OK);
	if(IsSupported_())
		response.setContentType(supported_files_.at(requested_path_->getExtension()).get_content_type());
	else
		response.setContentType("application/octet-stream");

	std::ostream& out = response.send();
	std::ifstream out_file(requested_path_->toString(), std::ios::binary | std::ios::ate);

	auto size = out_file.tellg();
	std::string text_line(size, '\0');
	out_file.seekg(0);
	response.setContentLength(size);
	if(out_file.read(&text_line[0], size))
	{
		out << text_line;
	}

	out_file.close();
	out.flush();
}

void FrontendHandler::ManageTextPlainFile_(HTTPServerResponse& response)
{
	response.setStatus(HTTPResponse::HTTP_OK);
	if(IsSupported_())
		response.setContentType(supported_files_.at(requested_path_->getExtension()).get_content_type());
	else
		response.setContentType("text/plain");

	std::string text_line;
	std::ostream& out = response.send();
	std::ifstream out_file(requested_path_->toString());
	while (getline (out_file, text_line))
	{
		out << text_line;
	}
	out_file.close();
	out.flush();
}
