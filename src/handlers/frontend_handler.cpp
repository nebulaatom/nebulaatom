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
    get_routes_list().push_back({});
    get_routes_list().push_back({std::vector<std::string>{"uploaded-files"}});
}

void FrontendHandler::Process_()
{
    SettingUpFunctions_();
}

void FrontendHandler::HandleGETMethod_()
{
    // Manage the file
        auto tmp_uri = URI(get_request()->getURI());
        auto tmp_file = Files::File("file", Path(tmp_uri.getPath()).getFileName(), "", 0);
        tmp_file.get_requested_path().reset(new Path(file_manager_.get_directory_base() + tmp_uri.getPath(), Path::PATH_NATIVE));
        tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));

        file_manager_.get_files().push_back(tmp_file);

    // Basic operations
        file_manager_.set_operation_type(Files::OperationType::kDownload);
        if(!file_manager_.CheckFiles_())
        {
            HTMLResponse_(*get_response(), HTTPResponse::HTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }
        if(!file_manager_.IsSupported_(file_manager_.get_files().front()))
        {
            HTMLResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }

        file_manager_.ProcessFileType_();
        file_manager_.ProcessContentLength_(file_manager_.get_files().front());

    // Reponse
        get_response()->setStatus(HTTPResponse::HTTP_OK);
        get_response()->setContentType(file_manager_.get_files().front().get_content_type());
        get_response()->setContentLength(file_manager_.get_files().front().get_content_length());
        std::ostream& out_reponse = get_response()->send();

    // Download file
        file_manager_.DownloadFile_(out_reponse);

    out_reponse.flush();
}

void FrontendHandler::HandlePOSTMethod_()
{
    // Manage the file
        file_manager_.set_operation_type(Files::OperationType::kUpload);
        HTMLForm form(*get_request(), get_request()->stream(), file_manager_);

    // Basic operations
        for(auto& file_it : file_manager_.get_files())
        {
            if(!file_manager_.IsSupported_(file_it))
            {
                HTMLResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, "Requested file is not supported.");
                return;
            }
            app_.logger().information("File: " + file_it.get_requested_file()->path());
        }

        file_manager_.ProcessFileType_();
        file_manager_.UploadFile_();

    // Response
        get_response()->setStatus(HTTPResponse::HTTP_OK);
        get_response()->setContentType("application/json");
        get_response()->setChunkedTransferEncoding(true);

        std::ostream& out_reponse = get_response()->send();

        file_manager_.get_result()->stringify(out_reponse);

        out_reponse.flush();
}

void FrontendHandler::HandlePUTMethod_()
{
    // Manage the file
        auto tmp_file = Files::File("file", Path(get_request()->getURI()).getFileName(), "", 0);
        tmp_file.get_requested_path().reset(new Path(file_manager_.get_directory_base() + get_request()->getURI(), Path::PATH_NATIVE));
        tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));

        file_manager_.get_files().push_back(tmp_file);

    // Basic operations
        file_manager_.set_operation_type(Files::OperationType::kDelete);
        if(!file_manager_.CheckFiles_())
        {
            HTMLResponse_(*get_response(), HTTPResponse::HTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }

    // Remove the file
        file_manager_.RemoveFile_();
        file_manager_.get_files().clear();

    // Upload
        file_manager_.get_result()->clear();
        file_manager_.set_operation_type(Files::OperationType::kUpload);
        HTMLForm form(*get_request(), get_request()->stream(), file_manager_);

        if(!file_manager_.IsSupported_(file_manager_.get_files().front()))
        {
            HTMLResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }
        app_.logger().information("File: " + file_manager_.get_files().front().get_requested_file()->path());

        file_manager_.ProcessFileType_();
        file_manager_.UploadFile_();

    // Response
        get_response()->setStatus(HTTPResponse::HTTP_OK);
        get_response()->setContentType("application/json");
        get_response()->setChunkedTransferEncoding(true);

        std::ostream& out_reponse = get_response()->send();

        file_manager_.get_result()->stringify(out_reponse);

        out_reponse.flush();
}

void FrontendHandler::HandleDELMethod_()
{
    // Manage file
        auto tmp_file = Files::File("file", Path(get_request()->getURI()).getFileName(), "", 0);
        tmp_file.get_requested_path().reset(new Path(file_manager_.get_directory_base() + get_request()->getURI(), Path::PATH_NATIVE));
        tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));

        file_manager_.get_files().push_back(tmp_file);

    // Basic operations
        file_manager_.set_operation_type(Files::OperationType::kDelete);
        if(!file_manager_.CheckFiles_())
        {
            HTMLResponse_(*get_response(), HTTPResponse::HTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }

        file_manager_.RemoveFile_();

    // Response
        get_response()->setStatus(HTTPResponse::HTTP_OK);
        get_response()->setContentType("application/json");
        get_response()->setChunkedTransferEncoding(true);

        std::ostream& out_reponse = get_response()->send();

        file_manager_.get_result()->stringify(out_reponse);

        out_reponse.flush();
}
