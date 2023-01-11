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

#include "handlers/root_handler.h"

using namespace CPW::Handlers;

RootHandler::RootHandler(std::shared_ptr<Tools::SessionsHandler> sessions_handler, std::string api_version) :
    app_(Application::instance())
    ,api_version_(api_version)
    ,route_verification_(true)
    ,dynamic_elements_(new Extras::DynamicElements())
    ,sessions_handler_(sessions_handler)
{
    requests_manager_.set_http_methods(*this);
    requests_manager_.get_http_methods()->set_dynamic_elements(dynamic_elements_);
    HTTPMethods::set_dynamic_elements(dynamic_elements_);
}

RootHandler::~RootHandler()
{

}

void RootHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    try
    {
        // Process request and response
            dynamic_elements_->set_request(request);
            dynamic_elements_->set_response(response);
            if(dynamic_elements_->get_request() == nullptr || dynamic_elements_->get_response() == nullptr)
                GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Something was wrong with the request or response.");

        // Process route
            if(!ProcessRoute_())
                return;

        // Found the corresponding HTTP method
            auto method = requests_manager_.get_actions_strings().find(dynamic_elements_->get_request()->getMethod());
            if(method == requests_manager_.get_actions_strings().end())
                GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "The client provided a bad HTTP method.");

        // Call the corresponding HTTP method
            requests_manager_.get_actions_strings()[dynamic_elements_->get_request()->getMethod()]();
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + error.displayText());
        GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_BAD_REQUEST, "Error with the database or query. " + error.displayText());
    }
    catch(RuntimeException& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + error.displayText());
        GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + error.displayText());
        GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(std::out_of_range& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
        GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
        GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
        GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
}

bool RootHandler::ProcessRoute_()
{
        AddRoutes_();

        std::vector<std::string> segments;
        URI(dynamic_elements_->get_request()->getURI()).getPathSegments(segments);
        dynamic_elements_->set_requested_route(std::make_shared<Tools::Route>("", segments));

        switch(dynamic_elements_->get_requested_route()->get_current_route_type())
        {
            case CPW::Tools::RouteType::kEndpoint:
            {
                if(!ManageRequestBody_())
                {
                    GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_BAD_REQUEST, "Something was wrong with the Request body.");
                    return false;
                }

                std::vector<std::string> login_route({"api", api_version_, "system", "login"});
                std::vector<std::string> logout_route({"api", api_version_, "system", "logout"});

                if
                (
                    dynamic_elements_->get_requested_route()->get_segments() != login_route
                    && dynamic_elements_->get_requested_route()->get_segments() != logout_route
                )
                {
                    if(route_verification_)
                    {
                        if(!IdentifyRoute_())
                        {
                            GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_NOT_FOUND, "The requested endpoint is not available.");
                            return false;
                        }
                    }

                    if(!InitSecurityProccess_())
                        return false;

                }

                break;
            }
            case CPW::Tools::RouteType::kEntrypoint:
            {
                break;
            }
        }

    return true;
}

bool RootHandler::InitSecurityProccess_()
{
    current_security_.get_dynamic_elements().set_requested_route(dynamic_elements_->get_requested_route());
    if(current_security_.AuthenticateUser_())
    {
        if(!current_security_.VerifyPermissions_(dynamic_elements_->get_request()->getMethod()))
        {
            GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_UNAUTHORIZED, "The user does not have the permissions to perform this operation.");
            return false;
        }
    }
    else
    {
        GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_UNAUTHORIZED, "Unauthorized user or wrong user or password.");
        return false;
    }

    return true;
}

bool RootHandler::IdentifyRoute_()
{
    for(auto& it : dynamic_elements_->get_routes_list())
    {
        if(dynamic_elements_->get_requested_route()->SegmentsToString_() == it.SegmentsToString_())
        {
            dynamic_elements_->get_requested_route()->set_target(it.get_target());
            return true;
        }
    }

    return false;
}

bool RootHandler::ManageRequestBody_()
{
    auto query_actions = dynamic_elements_->get_query_actions();
    std::string request_body = query_actions->ReadBody_(dynamic_elements_->get_request()->stream());

    if(request_body.empty())
    {
        URI tmp_uri(dynamic_elements_->get_request()->getURI());
        if(!(tmp_uri.getQueryParameters().size() > 0))
            return false;

        if(tmp_uri.getQueryParameters()[0].first != "json")
            return false;

        if(tmp_uri.getQueryParameters()[0].second.empty())
            return false;

        request_body = tmp_uri.getQueryParameters()[0].second;
    }

    if(!dynamic_elements_->get_query_actions()->Parse_(request_body))
        return false;

    current_security_.get_dynamic_elements().get_query_actions()->get_json_body() = query_actions->get_json_body();

    return true;
}
