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
    out.flush();
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
    out.flush();
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
        out <<
            "<html>"
                "<head><title>" << responses_.find(status)->second.status_int << " " << responses_.find(status)->second.message << " | Nebula Atom</title></head>"
                "<body>"
                    "<center><h1>Status: " << responses_.find(status)->second.status_int << " " << responses_.find(status)->second.message << "</h1></center>"
                    "<center><h3>Message: " << message << "</h3></center>"
                    "<center><hr>Nebula Atom/" << PACKAGE_VERSION_COMPLETE << "</center>"
                "</body>"
            "</html>"
        ;
    }
    else
    {
        out <<
            "<html>"
                "<head><title>" << responses_.find(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR)->second.message << " | Nebula Atom</title></head>"
                "<body>"
                    "<center><h1>Status: 500 " << responses_.find(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR)->second.message << "</h1></center>"
                    "<center><h3>Message: " << "Error in HTTPStatus." << "</h3></center>"
                    "<center>Nebula Atom/" << PACKAGE_VERSION_COMPLETE << "</center>"
                "</body>"
            "</html>"
        ;
    }

    out.flush();
}

void CommonResponses::CustomHTMLResponse_(HTTP::Status status, std::string html_message)
{
    response_->setStatus(responses_.find(status)->second.http_status);
    response_->setContentType("text/html");
    response_->setChunkedTransferEncoding(true);

    std::ostream& out = response_->send();
    out << html_message;
    out.flush();
}

void CommonResponses::FileResponse_(HTTP::Status status, std::string address)
{
    // Manage the file
        Files::FileManager file_manager;

        auto tmp_file = Files::File("file", address, "", 0);
        tmp_file.get_requested_path().reset(new Path(address, Path::PATH_NATIVE));
        tmp_file.get_requested_file().reset(new Poco::File(*tmp_file.get_requested_path()));

        file_manager.get_files().push_back(tmp_file);

    // Basic operations
        file_manager.set_operation_type(Files::OperationType::kDownload);
        if(!file_manager.CheckFiles_())
        {
            HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
            return;
        }
        if(!file_manager.IsSupported_(file_manager.get_files().front()))
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
            return;
        }

        file_manager.ProcessFileType_();
        file_manager.ProcessContentLength_(file_manager.get_files().front());

    // Reponse
        response_->setStatus(responses_.find(status)->second.http_status);
        response_->setContentType(file_manager.get_files().front().get_content_type());
        response_->setContentLength(file_manager.get_files().front().get_content_length());
        std::ostream& out_reponse = response_->send();

    // Download file
        file_manager.DownloadFile_(out_reponse);

    out_reponse.flush();
}

void CommonResponses::FillResponses_()
{
    responses_.emplace(std::make_pair(Status::kOK, Attributes{HTTPResponse::HTTP_OK, 200,ResponseType::kSuccess, "Ok"}));
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
