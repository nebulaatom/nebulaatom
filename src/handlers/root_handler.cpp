/*
* Nebula Atom
* Copyright (C) 2021 NebulaCSF support@cpwonline.org
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

#include "handlers/root_handler.h"
#include "http/common_responses.h"

using namespace AtomHandlers;

RootHandler::RootHandler() :
    app_(Application::instance())
    ,user_("null")
    ,method_("GET")
    ,route_verification_(true)
    ,request_(nullptr)
    ,response_(nullptr)
    ,current_function_()
{
    requested_route_ = std::make_shared<Tools::Route>(std::vector<std::string>{""});
    current_security_.set_security_type(Extras::SecurityType::kDisableAll);
}

RootHandler::~RootHandler()
{

}

void RootHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    try
    {
        // Set request and response
            request_ = &request;
            response_ = &response;
            HTTP::CommonResponses::set_response(response_);
            
            if(request_ == nullptr || response_ == nullptr)
            {
                JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Request or response is Null Pointer.");
                return;
            }

        // Set requested route
            std::vector<std::string> segments;
            URI(request_->getURI()).getPathSegments(segments);
            requested_route_ = std::make_shared<Tools::Route>(segments);

        // Add functions
            AddFunctions_();

        // Process the request body
            ManageRequestBody_();

        // Get the corresponding HTTP method
            method_ = request.getMethod();

        // Handler Process
            Process_();
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error with the database or query. " + error.displayText());
    }
    catch(RuntimeException& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(std::out_of_range& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(...)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): No handled exception.");
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. No handled exception." );
    }
}

void RootHandler::CallHTTPMethod_()
{
    Functions::Function f1("", Functions::Function::Type::kGET);
    auto found = f1.get_methods().find(get_method());
    if(found == f1.get_methods().end())
    {
        JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Method not found.");
        return;
    }

    switch(found->second)
    {
        case Functions::Function::Type::kPOST: HandlePOSTMethod_(); break;
        case Functions::Function::Type::kGET: HandleGETMethod_(); break;
        case Functions::Function::Type::kPUT: HandlePUTMethod_(); break;
        case Functions::Function::Type::kDEL: HandleDELMethod_(); break;
        default:
        {
            JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Method not found.");
            return;
        }
    }
}

bool RootHandler::ProcessRoute_()
{/*
    // Prepare routes
        AddFunctions_();

        std::vector<std::string> segments;
        URI(request_->getURI()).getPathSegments(segments);
        requested_route_ = std::make_shared<Tools::Route>(segments);

    // Manage the route type
        switch(requested_route_->get_current_route_type())
        {
            case AtomTools::RouteType::kEndpoint:
            {
                current_security_.set_security_type(Extras::SecurityType::kDisableAll);

                // Process the request body
                    if(!ManageRequestBody_())
                    {
                        JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Something was wrong with the Request body.");
                        return false;
                    }

                // Route identification and identification
                    std::vector<std::string> login_route({"api", api_version_, "system", "login"});
                    std::vector<std::string> logout_route({"api", api_version_, "system", "logout"});

                    if(requested_route_->get_segments() != login_route && requested_route_->get_segments() != logout_route)
                    {
                        if(route_verification_)
                        {
                            if(!IdentifyRoute_())
                            {
                                JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint is not available.");
                                return false;
                            }
                        }

                        if(!VerifySession_())
                        {
                            JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Session not found.");
                            return false;
                        }

                        if(!VerifyPermissions_())
                            return false;

                    }

                break;
            }
            case AtomTools::RouteType::kEntrypoint:
            {
                current_security_.set_security_type(Extras::SecurityType::kEnableAll);

                VerifySession_();

                if(!VerifyPermissions_())
                    return false;

                break;
            }
        }
*/
    return true;
}

bool RootHandler::VerifySession_()
{
    // Extract session ID
        std::string session_id;
        Poco::Net::NameValueCollection cookies;
        request_->getCookies(cookies);
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
                user_ = sessions.at(session_id).get_user();

            return true;
        }
        else
            return false;
}

bool RootHandler::VerifyPermissions_()
{
    // Setting up user
        current_security_.get_users_manager().get_current_user().set_username(user_);

    // Verify permissions
    if(!current_security_.VerifyRoutesPermissions_(*requested_route_, method_))
    {
        JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "The user does not have the permissions to perform this operation.");
        return false;
    }

    return true;
}

bool RootHandler::IdentifyRoute_()
{
    for(auto& it : routes_list_)
    {
        if(requested_route_->SegmentsToString_() == it.SegmentsToString_())
        {
            // Setting up the route functions
            auto endpoint = requested_route_->SegmentsToString_();
            auto found = functions_manager_.get_functions().find(endpoint);
            if(found == functions_manager_.get_functions().end())
                return false;

            // Validate type
            auto found_type = found->second.get_methods().find(method_);
            if(found_type == found->second.get_methods().end())
                return false;

            // Same type
            if(found_type->second != found->second.get_type())
                continue;

            // Copy function and reset results
            current_function_ = found->second;
            for(auto& action : current_function_.get_actions())
            {
                action->get_results() = std::make_shared<Query::Results>();
            }

            return true;
        }
    }

    return false;
}

void RootHandler::ManageRequestBody_()
{
    std::string request_body = ReadBody_(request_->stream());

    if(request_body.empty())
    {
        URI tmp_uri(request_->getURI());
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

void RootHandler::AddFunctions_()
{

}

void RootHandler::Process_()
{
    // Use SSL
        /*SecureStreamSocket socket = static_cast<Net::HTTPServerRequestImpl&>(request).socket();
        if (socket.havePeerCertificate())
            X509Certificate cert = socket.peerCertificate();*/

    // Process route
        if(!ProcessRoute_())
            return;

}

void RootHandler::HandleGETMethod_()
{

}

void RootHandler::HandlePOSTMethod_()
{

}

void RootHandler::HandlePUTMethod_()
{

}

void RootHandler::HandleDELMethod_()
{

}
