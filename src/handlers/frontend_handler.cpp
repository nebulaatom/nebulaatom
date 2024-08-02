/*
* Nebula Atom

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

using namespace NAF::Handlers;

FrontendHandler::FrontendHandler() :
    RootHandler()
{

}

FrontendHandler::~FrontendHandler()
{

}

void FrontendHandler::Process_()
{
    file_manager_.AddBasicSupportedFiles_();
    auto method = GetMethod_(get_http_server_request().value()->getMethod());
    switch(method)
    {
        case HTTP::EnumMethods::kHTTP_GET:
            DownloadProcess_();
            break;
        case HTTP::EnumMethods::kHTTP_POST:
            UploadProcess_();
            break;
        case HTTP::EnumMethods::kHTTP_PUT:
            ModifyProcess_();
            break;
        case HTTP::EnumMethods::kHTTP_DEL:
            RemoveProcess_();
            break;
        case HTTP::EnumMethods::kHTTP_HEAD:
        case HTTP::EnumMethods::kHTTP_OPTIONS:
        case HTTP::EnumMethods::kHTTP_PATCH:
        case HTTP::EnumMethods::kNULL:
            JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
            break;
    }
}

void FrontendHandler::DownloadProcess_()
{
    // Manage the file

        auto& request = get_http_server_request().value();
        file_manager_.set_operation_type(Files::OperationType::kDownload);
        file_manager_.get_files().push_back(file_manager_.CreateTempFile_(request->getURI()));
        auto& tmp_file = file_manager_.get_files().front();

    // Check file
        if(!file_manager_.CheckFiles_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }

    // Is supported
        if(!file_manager_.IsSupported_(tmp_file))
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }
        file_manager_.ProcessContentLength_();

    // Reponse
        auto& response = get_http_server_response().value();
        response->setStatus(HTTPResponse::HTTP_OK);
        response->setContentType(tmp_file.get_content_type());
        response->setContentLength(tmp_file.get_content_length());
        
        std::ostream& out_reponse = response->send();

    // Download file
        file_manager_.DownloadFile_(out_reponse);
}

void FrontendHandler::UploadProcess_()
{
    // Manage the files
        auto& request = get_http_server_request().value();
        file_manager_.set_operation_type(Files::OperationType::kUpload);
        HTMLForm form(*request, request->stream(), file_manager_);

    // Verify supported files
        if(!file_manager_.IsSupported_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }

    // Verify max file size
        if(!file_manager_.VerifyMaxFileSize_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The requested file exceeds the file size limit.");
            return;
        }

    // Upload file
        file_manager_.UploadFile_();

    // Response
        CompoundFillResponse_(HTTP::Status::kHTTP_OK, file_manager_.get_result(), "Ok.");
}

void FrontendHandler::ModifyProcess_()
{
    // Manage the file
        auto& request = get_http_server_request().value();
        file_manager_.set_operation_type(Files::OperationType::kDelete);
        file_manager_.get_files().push_back(file_manager_.CreateTempFile_(request->getURI()));

    // Check file
        if(!file_manager_.CheckFiles_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }

    // Remove the file
        file_manager_.RemoveFile_();
        file_manager_.get_files().clear();
        file_manager_.get_result()->clear();

    // Process new file to upload
        file_manager_.set_operation_type(Files::OperationType::kUpload);
        HTMLForm form(*request, request->stream(), file_manager_);

        if(!file_manager_.IsSupported_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }
        
    // Verify max file size
        if(!file_manager_.VerifyMaxFileSize_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The requested file exceeds the file size limit.");
            return;
        }

    // Upload
        file_manager_.UploadFile_();

    // Response
        CompoundFillResponse_(HTTP::Status::kHTTP_OK, file_manager_.get_result(), "Ok.");
}

void FrontendHandler::RemoveProcess_()
{
    // Manage file
        auto& request = get_http_server_request().value();
        file_manager_.set_operation_type(Files::OperationType::kDelete);
        file_manager_.get_files().push_back(file_manager_.CreateTempFile_(request->getURI()));

    // Basic operations
        if(!file_manager_.CheckFiles_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }

    // Remove
        file_manager_.RemoveFile_();
        
    // Response
        JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
}
