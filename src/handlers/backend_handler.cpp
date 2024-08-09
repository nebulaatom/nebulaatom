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

#include "handlers/backend_handler.h"
#include "files/file.h"
#include "files/file_manager.h"
#include "functions/function.h"
#include "tools/output_logger.h"

using namespace NAF::Handlers;

BackendHandler::BackendHandler() :
    RootHandler::RootHandler()
{

}

void BackendHandler::Process_()
{
    ProcessActions_();
}

void BackendHandler::ProcessActions_()
{
    // Verify current function
    if(get_current_function()->get_actions().empty())
    {
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Current function has no actions.");
        return;
    }
    // Identify parameters
    IdentifyParameters_();

    // Process current function

    switch(get_current_function()->get_response_type())
    {
        case Functions::Function::ResponseType::kJSON:
        {
            ProcessJSONResponse_();
            break;
        }
        case Functions::Function::ResponseType::kFile:
        {
            ProcessFileResponse_();
            break;
        }
    }
}

void BackendHandler::ProcessJSONResponse_()
{
    JSON::Object::Ptr json_result = new JSON::Object();
    if(!get_current_function()->ProcessJSON_(json_result))
    {
        if(get_current_function()->get_error())
        {
            JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, get_current_function()->get_error_message());
            return;
        }
    }

    // Send JSON results
    CompoundResponse_(HTTP::Status::kHTTP_OK, json_result);
}

void BackendHandler::ProcessFileResponse_()
{
    std::string filepath = "";
    if(!get_current_function()->ProcessFile_(filepath))
    {
        if(get_current_function()->get_error())
        {
            JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, get_current_function()->get_error_message());
            return;
        }
    }

    // Manage the file
    file_manager_.set_operation_type(Files::OperationType::kDownload);
    file_manager_.get_files().push_back(file_manager_.CreateTempFile_("/" + filepath));
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