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

using namespace Atom::Handlers;

LoginHandler::~LoginHandler()
{

}

void LoginHandler::AddFunctions_()
{
    auto& routes_list = get_routes_list();

    routes_list.push_back({{"api", "system", "login"}});
    routes_list.push_back({{"api", "system", "logout"}});

}

void LoginHandler::Process_()
{
    CallHTTPMethod_();
}

void LoginHandler::HandleGETMethod_()
{
    JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
}

void LoginHandler::HandlePOSTMethod_()
{
    // Process the request body
    ManageRequestBody_();

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

void LoginHandler::HandlePUTMethod_()
{
    JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
}

void LoginHandler::HandleDELMethod_()
{
    JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
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

    // Get the user and password
        std::string user = "", password = "";

        if(get_json_body()->size() > 0)
        {
            auto json_auth = get_json_body()->getObject(0);
            if(!json_auth->get("user").isEmpty())
                user = json_auth->get("user").toString();
            if(!json_auth->get("password").isEmpty())
                password =  json_auth->get("password").toString();
        }

    // Security verification
        get_current_security().get_users_manager().get_current_user().set_username(user);
        get_current_security().get_users_manager().get_current_user().set_password(password);
        if(!get_current_security().get_users_manager().AuthenticateUser_())
        {
            JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Unauthorized user or wrong user or password.");
            return;
        }

    // Create the session
        auto session = Tools::SessionsManager::CreateSession_(user, "/", get_settings_manager().get_basic_properties_().session_max_age);

    // Response
        Poco::Net::HTTPCookie cookie("nebula-atom-sid", session.get_id());
        cookie.setPath(session.get_path());
        cookie.setMaxAge(session.get_max_age());
        cookie.setSameSite(Poco::Net::HTTPCookie::SAME_SITE_STRICT);
        cookie.setHttpOnly(true);
        cookie.setSecure(true);

        get_response()->addCookie(cookie);

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
        get_response()->addCookie(cookie);

        JSONResponse_(HTTP::Status::kHTTP_OK, "Client logout.");
}

std::string LoginHandler::SessionExists_()
{
    // Extract session ID
        std::string session_id;
        Poco::Net::NameValueCollection cookies;
        get_request()->getCookies(cookies);
        auto cookie_session = cookies.find("nebula-atom-sid");
        auto sessions = Tools::SessionsManager::get_sessions();

    // Session exists
        if(cookie_session == cookies.end())
            return "";

        if(sessions.find(cookie_session->second) == sessions.end())
            return "";

        return sessions.find(cookie_session->second)->first;
}
