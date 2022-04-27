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

#include "handlers/frontend_handler.h"

using namespace CPW::Handlers;

FrontendHandler::FrontendHandler(std::string api_version) :
	RootHandler(api_version)
{

}

FrontendHandler::~FrontendHandler()
{

}

void FrontendHandler::AddRoutes_()
{
	get_dynamic_elements()->get_routes_list()->push_back({"",{""}});
	get_dynamic_elements()->get_routes_list()->push_back({"uploaded-files",{"uploaded-files"}});
}

void FrontendHandler::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
    // Manage the file
        auto tmp_file = Extras::File("file", Path(request.getURI()).getFileName(), "", 0);
        tmp_file.get_requested_path().reset(new Path(file_manager_.get_directory_base() + request.getURI(), Path::PATH_NATIVE));
        tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));

        file_manager_.get_files().push_back(tmp_file);

	// Basic operations
        file_manager_.set_operation_type(Tools::OperationType::kDownload);
        if(!file_manager_.CheckFiles_())
        {
            GenericResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }
        if(!file_manager_.IsSupported_(file_manager_.get_files().front()))
        {
            GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }

        file_manager_.ProcessFileType_();
        file_manager_.ProcessContentLength_(file_manager_.get_files().front());

    // Reponse
        response.setStatus(HTTPResponse::HTTP_OK);
        response.setContentType(file_manager_.get_files().front().get_content_type());
        response.setContentLength(file_manager_.get_files().front().get_content_length());
        std::ostream& out_reponse = response.send();

    // Download file
        file_manager_.DownloadFile_(out_reponse);

	out_reponse.flush();
}

void FrontendHandler::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
    // Manage the file
        file_manager_.set_operation_type(Tools::OperationType::kUpload);
        HTMLForm form(request, request.stream(), file_manager_);

	// Basic operations
        for(auto& file_it : file_manager_.get_files())
        {
            if(!file_manager_.IsSupported_(file_it))
            {
                GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "Requested file is not supported.");
                return;
            }
            app_.logger().information("File: " + file_it.get_requested_file()->path());
        }

        file_manager_.ProcessFileType_();
        file_manager_.UploadFile_();

    // Response
        response.setStatus(HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        response.setChunkedTransferEncoding(true);

        std::ostream& out_reponse = response.send();

        file_manager_.get_result()->stringify(out_reponse);

        out_reponse.flush();
}

void FrontendHandler::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
    // Manage the file
        auto tmp_file = Extras::File("file", Path(request.getURI()).getFileName(), "", 0);
        tmp_file.get_requested_path().reset(new Path(file_manager_.get_directory_base() + request.getURI(), Path::PATH_NATIVE));
        tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));

        file_manager_.get_files().push_back(tmp_file);

    // Basic operations
        file_manager_.set_operation_type(Tools::OperationType::kDelete);
        if(!file_manager_.CheckFiles_())
        {
            GenericResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }

    // Remove the file
        file_manager_.RemoveFile_();
        file_manager_.get_files().clear();

    // Upload
        file_manager_.get_result()->clear();
        file_manager_.set_operation_type(Tools::OperationType::kUpload);
        HTMLForm form(request, request.stream(), file_manager_);

        if(!file_manager_.IsSupported_(file_manager_.get_files().front()))
        {
            GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }
        app_.logger().information("File: " + file_manager_.get_files().front().get_requested_file()->path());

        file_manager_.ProcessFileType_();
        file_manager_.UploadFile_();

    // Response
        response.setStatus(HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        response.setChunkedTransferEncoding(true);

        std::ostream& out_reponse = response.send();

        file_manager_.get_result()->stringify(out_reponse);

        out_reponse.flush();
}

void FrontendHandler::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
    // Manage file
        auto tmp_file = Extras::File("file", Path(request.getURI()).getFileName(), "", 0);
        tmp_file.get_requested_path().reset(new Path(file_manager_.get_directory_base() + request.getURI(), Path::PATH_NATIVE));
        tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));

        file_manager_.get_files().push_back(tmp_file);

    // Basic operations
        file_manager_.set_operation_type(Tools::OperationType::kDelete);
        if(!file_manager_.CheckFiles_())
        {
            GenericResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }

        file_manager_.RemoveFile_();

    // Response
        response.setStatus(HTTPResponse::HTTP_OK);
        response.setContentType("application/json");
        response.setChunkedTransferEncoding(true);

        std::ostream& out_reponse = response.send();

        file_manager_.get_result()->stringify(out_reponse);

        out_reponse.flush();
}
