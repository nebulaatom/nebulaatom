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

#include "handlers/root_handler.h"

using namespace Atom::Handlers;

RootHandler::RootHandler() :
    current_function_()
{
    requested_route_ = std::make_shared<Tools::Route>(std::vector<std::string>{""});
    set_security_type(Extras::SecurityType::kDisableAll);
}

RootHandler::~RootHandler()
{

}

void RootHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    try
    {
        // Set request and response
            SetupRequest_(request);
            SetupResponse_(response);
            
            if(!get_http_server_request().has_value())
            {
                JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "HTTP Request is Null.");
                return;
            }
            if(!get_http_server_response().has_value())
            {
                JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "HTTP Response is Null.");
                return;
            }

        // Set requested route
            std::vector<std::string> segments;
            URI(request.getURI()).getPathSegments(segments);
            requested_route_ = std::make_shared<Tools::Route>(segments);

        // Handler Process
            Process_();
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::Log_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error with the database or query. " + error.displayText());
    }
    catch(RuntimeException& error)
    {
        Tools::OutputLogger::Log_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        Tools::OutputLogger::Log_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(std::out_of_range& error)
    {
        Tools::OutputLogger::Log_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Log_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Log_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(...)
    {
        Tools::OutputLogger::Log_("Error on root_handler.cpp on handleRequest(): No handled exception.");
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. No handled exception." );
    }
}

bool RootHandler::SetupSSL_()
{
    auto request = get_http_server_request().value();
    SecureStreamSocket socket = static_cast<Net::HTTPServerRequestImpl&>(*request).socket();
    if (socket.havePeerCertificate())
        X509Certificate cert = socket.peerCertificate();

    return true;
}

Atom::Functions::Function::Ptr RootHandler::AddFunction_(std::string endpoint, HTTP::EnumMethods method)
{
    return functions_manager_.AddFunction_(endpoint, method);
}

bool RootHandler::VerifySession_()
{
    // Extract session ID
        auto request = get_http_server_request().value();
        std::string session_id;
        Net::NameValueCollection cookies;
        request->getCookies(cookies);
        auto cookie_session = cookies.find("nebula-atom-sid");
        auto sessions = Tools::SessionsManager::get_sessions();

    // Verify Cookie session and session
        if(cookie_session != cookies.end())
        {
            session_id = cookie_session->second;
            if(sessions.find(session_id) == sessions.end())
            {
                return false;
            }

            // Get the session user
                auto user = sessions.at(session_id).get_user();
                get_users_manager().get_current_user().set_username(user);

            return true;
        }
        else
            return false;
}

bool RootHandler::VerifyPermissions_()
{
    return VerifyRoutesPermissions_(*requested_route_, get_method());
}

bool RootHandler::IdentifyRoute_()
{
    for(auto& it : get_functions_manager().get_functions())
    {
        if(requested_route_->SegmentsToString_() == it.first)
        {
            // Verify same HTTP Method
            if(GetMethod_(get_method()) != it.second->get_method())
                continue;

            // Copy function and reset actions results
            current_function_ = it.second;
            for(auto& action : current_function_->get_actions())
                action->get_results() = std::make_shared<Query::Results>();

            return true;
        }
    }

    return false;
}

void RootHandler::ManageRequestBody_()
{
    auto request = get_http_server_request().value();
    std::string request_body = ReadBody_(request->stream());

    if(request_body.empty())
    {
        URI tmp_uri(request->getURI());
        if(!(tmp_uri.getQueryParameters().size() > 0))
            return;
        if(tmp_uri.getQueryParameters()[0].first != "json")
            return;
        if(tmp_uri.getQueryParameters()[0].second.empty())
            return;
        request_body = tmp_uri.getQueryParameters()[0].second;
    }

    Parse_(request_body);
}
