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

using namespace Atom::Handlers;

FrontendHandler::FrontendHandler() :
    RootHandler()
{

}

FrontendHandler::~FrontendHandler()
{

}

void FrontendHandler::AddFunctions_()
{
    
}

void FrontendHandler::Process_()
{
    CallHTTPMethod_();
}

void FrontendHandler::HandleGETMethod_()
{
    // Manage the file
        file_manager_.set_operation_type(Files::OperationType::kDownload);
        file_manager_.get_files().push_back(file_manager_.CreateTempFile_(get_request()->getURI()));
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
        get_response()->setStatus(HTTPResponse::HTTP_OK);
        get_response()->setContentType(tmp_file.get_content_type());
        get_response()->setContentLength(tmp_file.get_content_length());
        std::ostream& out_reponse = get_response()->send();

    // Download file
        file_manager_.DownloadFile_(out_reponse);
}

void FrontendHandler::HandlePOSTMethod_()
{
    // Manage the files
        file_manager_.set_operation_type(Files::OperationType::kUpload);
        HTMLForm form(*get_request(), get_request()->stream(), file_manager_);

    // Verify supported files
        if(!file_manager_.IsSupported_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }

    // Upload file
        file_manager_.UploadFile_();

    // Response
        CompoundFillResponse_(HTTP::Status::kHTTP_OK, file_manager_.get_result(), "Ok.");
}

void FrontendHandler::HandlePUTMethod_()
{
    // Manage the file
        file_manager_.set_operation_type(Files::OperationType::kDelete);
        file_manager_.get_files().push_back(file_manager_.CreateTempFile_(get_request()->getURI()));

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
        HTMLForm form(*get_request(), get_request()->stream(), file_manager_);

        if(!file_manager_.IsSupported_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }

    // Upload
        file_manager_.UploadFile_();

    // Response
        CompoundFillResponse_(HTTP::Status::kHTTP_OK, file_manager_.get_result(), "Ok.");
}

void FrontendHandler::HandleDELMethod_()
{
    // Manage file
        file_manager_.set_operation_type(Files::OperationType::kDelete);
        file_manager_.get_files().push_back(file_manager_.CreateTempFile_(get_request()->getURI()));

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
