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

using namespace NAF::Handlers;

RootHandler::RootHandler() :
    current_function_()
{
    requested_route_ = std::make_shared<Tools::Route>(std::vector<std::string>{""});
    set_security_type(Extras::SecurityType::kDisableAll);
    Query::DatabaseManager::Credentials credentials(
        Tools::SettingsManager::GetSetting_("db_host", "localhost")
        ,Tools::SettingsManager::GetSetting_("db_port", "3306")
        ,Tools::SettingsManager::GetSetting_("db_name", "db")
        ,Tools::SettingsManager::GetSetting_("db_user", "root")
        ,Tools::SettingsManager::GetSetting_("db_password", "root")
    );
    get_users_manager().get_credentials().Replace_(credentials);
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

            Tools::OutputLogger::Debug_(request.getURI() + " " + request.getMethod());
            
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

        // Add Server header
            AddHeader_("server", "nebulaatom/" + std::string(PACKAGE_VERSION_COMPLETE));

        // Set requested route
            std::vector<std::string> segments;
            URI(request.getURI()).getPathSegments(segments);
            requested_route_ = std::make_shared<Tools::Route>(segments);

        // Setup Properties
            SetupProperties_();

        // Handler Process
            Process_();
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error with the database or query. " + error.displayText());
    }
    catch(RuntimeException& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(std::out_of_range& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(...)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): No handled exception.");
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

NAF::Functions::Function::Ptr RootHandler::AddFunction_(std::string endpoint, HTTP::EnumMethods method)
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
    return VerifyRoutesPermissions_(*requested_route_, properties_.method);
}

bool RootHandler::IdentifyRoute_()
{
    for(auto& it : get_functions_manager().get_functions())
    {
        if(requested_route_->SegmentsToString_() == it.first)
        {
            // Verify same HTTP Method
            if(GetMethod_(properties_.method) != it.second->get_method())
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
    if(properties_.method == "POST" || properties_.method == "PUT")
    {
        if(properties_.content_type == "multipart/form-data")
        {
            ReadFormMultipart_(*request);
        }
        else if(properties_.content_type == "application/x-www-form-urlencoded")
        {
            ReadFormURLEncoded_(*request, request->stream());
        }
        else if(properties_.content_type == "application/json")
        {
            ReadJSON_(request->stream());
        }
    }
    else
    {
        std::string uri = request->getURI();
        ReadFromURI_(uri);
    }
}

void RootHandler::SetupProperties_()
{
    auto request = get_http_server_request().value();
    if(!get_http_server_request().has_value())
        return;

    properties_.method = IdentifyMethod_(request->getMethod());
    properties_.uri = request->getURI();
    properties_.content_type = request->getContentType();
    Net::MessageHeader::splitParameters(request->getContentType(), properties_.content_type, properties_.content_type_parameters);
}

void RootHandler::IdentifyParameters_(Functions::Action::Ptr action)
{
    switch(get_body_type())
    {
        case HTTP::Body::Type::kFormMultipart:
            action->IdentifyParameters_(get_form());
            action->IdentifyParameters_(*get_files_parameters());
            break;
        case HTTP::Body::Type::kJSON:
            action->IdentifyParameters_(get_json_array());
            break;
        case HTTP::Body::Type::kURI:
        case HTTP::Body::Type::kFormURLEncoded:
            action->IdentifyParameters_(get_query_parameters());
            if(get_json_array()->size() > 0)
                action->IdentifyParameters_(get_json_array());
            break;
    }
}

void RootHandler::IdentifyParameters_()
{
    auto& actions = current_function_->get_actions();
    for(auto it = actions.begin(); it != actions.end(); ++it)
    {
        IdentifyParameters_(*it);
    }
}