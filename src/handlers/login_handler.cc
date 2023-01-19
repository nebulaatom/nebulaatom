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

#include "handlers/login_handler.h"

using namespace CPW::Handlers;

LoginHandler::~LoginHandler()
{

}

void LoginHandler::HandleGETMethod_()
{
    GenericResponse_(*get_dynamic_elements()->get_response(), HTTPResponse::HTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
}

void LoginHandler::HandlePOSTMethod_()
{
    std::vector<std::string> login_route({"api", get_api_version(), "system", "login"});
    std::vector<std::string> logout_route({"api", get_api_version(), "system", "logout"});

    if(get_dynamic_elements()->get_requested_route()->get_segments() == login_route)
    {
        StartSession_();
    }
    else if(get_dynamic_elements()->get_requested_route()->get_segments() == logout_route)
    {
        EndSession_();
    }
    else
        GenericResponse_(*get_dynamic_elements()->get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Login route not identified.");
}

void LoginHandler::HandlePUTMethod_()
{
    GenericResponse_(*get_dynamic_elements()->get_response(), HTTPResponse::HTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
}

void LoginHandler::HandleDELMethod_()
{
    GenericResponse_(*get_dynamic_elements()->get_response(), HTTPResponse::HTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
}

void LoginHandler::AddRoutes_()
{
    auto& routes_list = get_dynamic_elements()->get_routes_list();

    routes_list.push_back({"login", {"api", get_api_version(), "system", "login"}});
    routes_list.push_back({"logout", {"api", get_api_version(), "system", "logout"}});

}

void LoginHandler::StartSession_()
{
    // Verify if the session exists
        auto session_id = SessionExists_();
        if(session_id != "")
        {
            GenericResponse_(*get_dynamic_elements()->get_response(), HTTPResponse::HTTP_OK, "The client has already logged in.");
            return;
        }

    // Get the user and password
        auto query_actions = get_dynamic_elements()->get_query_actions();
        auto json_auth = query_actions->get_json_body()->getArray("pair-information")->getObject(0)->getObject("auth");
        std::string user = "", password = "";

        if(!json_auth->get("user").isEmpty())
            user = json_auth->get("user").toString();
        if(!json_auth->get("password").isEmpty())
            password =  json_auth->get("password").toString();

    // Security verification
        get_current_security().set_user(user);
        get_current_security().set_password(password);
        get_current_security().get_dynamic_elements().set_requested_route(get_dynamic_elements()->get_requested_route());
        if(!get_current_security().AuthenticateUser_())
        {
            GenericResponse_(*get_dynamic_elements()->get_response(), HTTPResponse::HTTP_UNAUTHORIZED, "Unauthorized user or wrong user or password.");
            return;
        }

    // Create the session
        auto session = get_static_elements()->get_sessions_handler()->CreateSession_(user, "/", 2592000);

    // Response
        Poco::Net::HTTPCookie cookie("cpw-woodpecker-sid", session.get_id());
        cookie.setPath(session.get_path());
        cookie.setMaxAge(session.get_max_age());
        cookie.setSameSite(Poco::Net::HTTPCookie::SAME_SITE_STRICT);
        cookie.setHttpOnly(true);
        cookie.setSecure(true);

        get_dynamic_elements()->get_response()->addCookie(cookie);

        GenericResponse_(*get_dynamic_elements()->get_response(), HTTPResponse::HTTP_OK, "Client logged in.");
}

void LoginHandler::EndSession_()
{
    // Delete session
        auto session_id = SessionExists_();
        if (session_id != "")
        {
            get_static_elements()->get_sessions_handler()->DeleteSession_(session_id);
        }

    // Response
        Poco::Net::HTTPCookie cookie("cpw-woodpecker-sid", "");
        cookie.setPath("/");
        cookie.setMaxAge(-1);
        get_dynamic_elements()->get_response()->addCookie(cookie);

        GenericResponse_(*get_dynamic_elements()->get_response(), HTTPResponse::HTTP_OK, "Client logout.");
}

std::string LoginHandler::SessionExists_()
{
    // Extract session ID
        std::string session_id;
        Poco::Net::NameValueCollection cookies;
        get_dynamic_elements()->get_request()->getCookies(cookies);
        auto cookie_session = cookies.find("cpw-woodpecker-sid");
        auto sessions_handler = get_static_elements()->get_sessions_handler();

    // Session exists
        if(cookie_session == cookies.end())
            return "";

        if(sessions_handler->get_sessions().find(cookie_session->second) == sessions_handler->get_sessions().end())
            return "";

        return sessions_handler->get_sessions().find(cookie_session->second)->first;
}
