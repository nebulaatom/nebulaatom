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

#include "http/common_responses.h"
#include "files/file_manager.h"

using namespace Atom::HTTP;

CommonResponses::CommonResponses()
{
    FillResponses_();
}

CommonResponses::~CommonResponses()
{

}

void CommonResponses::CompoundResponse_(HTTP::Status status, JSON::Object::Ptr result_json)
{
    response_->setStatus(responses_.find(status)->second.http_status);
    response_->setContentType("application/json");
    response_->setChunkedTransferEncoding(true);

    std::ostream& out = response_->send();
    result_json->stringify(out);
}
void CommonResponses::CompoundFillResponse_(HTTP::Status status, JSON::Object::Ptr result_json, std::string message)
{
    response_->setStatus(responses_.find(status)->second.http_status);
    response_->setContentType("application/json");
    response_->setChunkedTransferEncoding(true);

    FillStatusMessage_(result_json, status, message);
    std::ostream& out = response_->send();
    result_json->stringify(out);
}

void CommonResponses::JSONResponse_(HTTP::Status status, std::string message)
{
    response_->setStatus(responses_.find(status)->second.http_status);
    response_->setContentType("application/json");
    response_->setChunkedTransferEncoding(true);

    JSON::Object::Ptr object_json = new JSON::Object;

    FillStatusMessage_(object_json, status, message);

    std::ostream& out = response_->send();
    object_json->stringify(out);
}

void CommonResponses::HTMLResponse_(HTTP::Status status, std::string message)
{
    response_->setStatus(responses_.find(status)->second.http_status);
    response_->setContentType("text/html");
    response_->setChunkedTransferEncoding(true);

    std::ostream& out = response_->send();

    auto found = responses_.find(status);
    if(found != responses_.end())
    {
        out << "<html>";
        out << "<head><title>" << responses_.find(status)->second.status_int << " " << responses_.find(status)->second.message << " | Nebula Atom</title></head>";
        out << "<body>";
        out << "<center><h1>Status: " << responses_.find(status)->second.status_int << " " << responses_.find(status)->second.message << "</h1></center>";
        out << "<center><h3>Message: " << message << "</h3></center>";
        out << "<center><hr>Nebula Atom/" << PACKAGE_VERSION_COMPLETE << "</center>";
        out << "</body>";
        out << "</html>";
    }
    else
    {
        out << "<html>";
        out << "<head><title>" << responses_.find(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR)->second.message << " | Nebula Atom</title></head>";
        out << "<body>";
        out << "<center><h1>Status: 500 " << responses_.find(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR)->second.message << "</h1></center>";
        out << "<center><h3>Message: " << "Error in HTTPStatus." << "</h3></center>";
        out << "<center>Nebula Atom/" << PACKAGE_VERSION_COMPLETE << "</center>";
        out << "</body>";
        out << "</html>";
    }
}

void CommonResponses::CustomHTMLResponse_(HTTP::Status status, std::string html_message)
{
    response_->setStatus(responses_.find(status)->second.http_status);
    response_->setContentType("text/html");
    response_->setContentLength(html_message.length());

    std::ostream& out = response_->send();
    out << html_message;
}

void CommonResponses::FileResponse_(HTTP::Status status, std::string address)
{
    // Manage the file
        Files::FileManager file_manager(Files::OperationType::kDownload);
        file_manager.get_files().push_back(file_manager.CreateTempFile_(address));
        auto tmp_file = file_manager.get_files().front();

    // Check file
        if(!file_manager.CheckFiles_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }
        if(!file_manager.IsSupported_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }
        
    // Reponse
        response_->setStatus(responses_.find(status)->second.http_status);
        response_->setContentType(file_manager.get_files().front().get_content_type());
        response_->setChunkedTransferEncoding(true);
        std::ostream& out_reponse = response_->send();

    // Download file
        file_manager.DownloadFile_(out_reponse);
}

void CommonResponses::FillResponses_()
{
    responses_.emplace(std::make_pair(Status::kHTTP_OK, Attributes{HTTPResponse::HTTP_OK, 200,ResponseType::kSuccess, "Ok"}));
    responses_.emplace(std::make_pair(Status::kHTTP_BAD_REQUEST, Attributes{HTTPResponse::HTTP_BAD_REQUEST, 400, ResponseType::kWarning, "Client-side input fails validation"}));
    responses_.emplace(std::make_pair(Status::kHTTP_UNAUTHORIZED, Attributes{HTTPResponse::HTTP_UNAUTHORIZED, 401, ResponseType::kWarning, "The user isn’t not authorized to access to this resource"}));
    responses_.emplace(std::make_pair(Status::kHTTP_FORBIDDEN, Attributes{HTTPResponse::HTTP_FORBIDDEN, 403, ResponseType::kWarning, "The user is not allowed to access to this resource"}));
    responses_.emplace(std::make_pair(Status::kHTTP_NOT_FOUND, Attributes{HTTPResponse::HTTP_NOT_FOUND, 404, ResponseType::kInformation, "Resource is not found"}));
    responses_.emplace(std::make_pair(Status::kHTTP_INTERNAL_SERVER_ERROR, Attributes{HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, 500, ResponseType::kError, "Something was wrong"}));
    responses_.emplace(std::make_pair(Status::kHTTP_BAD_GATEWAY, Attributes{HTTPResponse::HTTP_BAD_GATEWAY, 502, ResponseType::kError, "Invalid response from an upstream server"}));
    responses_.emplace(std::make_pair(Status::kHTTP_SERVICE_UNAVAILABLE, Attributes{HTTPResponse::HTTP_SERVICE_UNAVAILABLE, 503, ResponseType::kError, "Something unexpected happened on server side"}));
}

void CommonResponses::FillStatusMessage_(JSON::Object::Ptr json_object, HTTP::Status status, std::string message)
{
    auto found = responses_.find(status);
    if(found != responses_.end())
    {
        json_object->set("status", responses_.find(status)->second.message);
        json_object->set("message", message);
    }
    else
    {
        json_object->set("status", responses_.find(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR)->second.message);
        json_object->set("message", "Error on HTTPStatus");
    }
}
