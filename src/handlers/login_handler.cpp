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

#include "handlers/login_handler.h"

using namespace NAF::Handlers;

LoginHandler::~LoginHandler()
{

}

void LoginHandler::Process_()
{
    auto method = GetMethod_(get_http_server_request().value()->getMethod());
    switch(method)
    {
        case HTTP::EnumMethods::kHTTP_POST:
            Login_();
            break;
        case HTTP::EnumMethods::kHTTP_GET:
        case HTTP::EnumMethods::kHTTP_PUT:
        case HTTP::EnumMethods::kHTTP_DEL:
        case HTTP::EnumMethods::kHTTP_HEAD:
        case HTTP::EnumMethods::kHTTP_OPTIONS:
        case HTTP::EnumMethods::kHTTP_PATCH:
        case HTTP::EnumMethods::kNULL:
            JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
            break;
    }
}

void LoginHandler::Login_()
{
    std::vector<std::string> login_route({"api", "system", "login"});
    std::vector<std::string> logout_route({"api", "system", "logout"});

    if(get_requested_route()->get_segments() == login_route)
    {
        StartSession_();
    }
    else if(get_requested_route()->get_segments() == logout_route)
    {
        EndSession_();
    }
    else
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Login route not identified.");
}

void LoginHandler::StartSession_()
{
    // Verify if the session exists
        auto session_id = SessionExists_();
        if(session_id != "")
        {
            JSONResponse_(HTTP::Status::kHTTP_OK, "The client has already logged in.");
            return;
        }

        // Process the request body
        ManageRequestBody_();

        // Identify parameters
        IdentifyParameters_(get_users_manager().get_action());

        if(!get_users_manager().AuthenticateUser_())
        {
            JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Unauthorized user or wrong user or password.");
            return;
        }

    // Create the session
        auto session = Tools::SessionsManager::CreateSession_
        (
            get_users_manager().get_current_user().get_username()
            ,"/"
            ,Tools::SettingsManager::get_basic_properties_().session_max_age
        );

    // Response
        Poco::Net::HTTPCookie cookie("nebula-atom-sid", session.get_id());
        cookie.setPath(session.get_path());
        cookie.setMaxAge(session.get_max_age());
        cookie.setSameSite(Poco::Net::HTTPCookie::SAME_SITE_STRICT);
        cookie.setHttpOnly(true);
        cookie.setSecure(true);

        auto& response = get_http_server_response().value();
        response->addCookie(cookie);

        JSONResponse_(HTTP::Status::kHTTP_OK, "Client logged in.");
}

void LoginHandler::EndSession_()
{
    // Delete session
        auto session_id = SessionExists_();
        if (session_id != "")
        {
            Tools::SessionsManager::DeleteSession_(session_id);
        }

    // Response
        Poco::Net::HTTPCookie cookie("nebula-atom-sid", "");
        cookie.setPath("/");
        cookie.setMaxAge(-1);

        auto& response = get_http_server_response().value();
        response->addCookie(cookie);

        JSONResponse_(HTTP::Status::kHTTP_OK, "Client logout.");
}

std::string LoginHandler::SessionExists_()
{
    // Extract session ID
        std::string session_id;
        Poco::Net::NameValueCollection cookies;
        get_http_server_request().value()->getCookies(cookies);
        auto cookie_session = cookies.find("nebula-atom-sid");
        auto sessions = Tools::SessionsManager::get_sessions();

    // Session exists
        if(cookie_session == cookies.end())
            return "";

        if(sessions.find(cookie_session->second) == sessions.end())
            return "";

        return sessions.find(cookie_session->second)->first;
}
