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

#include "handler_factory.h"

using namespace CPW::Core;

HandlerFactory::HandlerFactory() :
    api_version_("v0")
    ,app_(Application::instance())
{
    CreateConnections_();
}

HandlerFactory::~HandlerFactory()
{

}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    try
    {
        std::vector<std::string> segments;

        URI(request.getURI()).getPathSegments(segments);

        CPW::Tools::Route requested_route(segments);

        switch(requested_route.get_current_route_type())
        {
            case CPW::Tools::RouteType::kEndpoint:
            {
                std::vector<std::string> login_route({"api", api_version_, "system", "login"});
                std::vector<std::string> logout_route({"api", api_version_, "system", "logout"});

                if
                (
                    requested_route.get_segments() == login_route
                    || requested_route.get_segments() == logout_route
                )
                    return connections_[HandlerType::kLogin]->return_handler_();
                else
                    return connections_[HandlerType::kBackend]->return_handler_();

                break;
            }
            case CPW::Tools::RouteType::kEntrypoint:
            {
                return connections_[HandlerType::kFrontend]->return_handler_();
                break;
            }
        }
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        GenericResponse_(request.response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Error with the database. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        GenericResponse_(request.response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(Poco::NullPointerException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        GenericResponse_(request.response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
        GenericResponse_(request.response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
        GenericResponse_(request.response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }

    return connections_[HandlerType::kNull]->return_handler_();
}

void HandlerFactory::CreateConnections_()
{
    connections_.insert(std::make_pair
    (
        HandlerType::kNull
        ,new Tools::HandlerConnection
        {
            CPW::Tools::Route(std::vector<std::string>{""})
            ,[&](){return new CPW::Handlers::NullHandler(api_version_);}
        }
    ));
    connections_.insert(std::make_pair
    (
        HandlerType::kBackend
        ,new Tools::HandlerConnection
        {
            CPW::Tools::Route(std::vector<std::string>{"api", api_version_})
            ,[&](){return new CPW::Handlers::BackendHandler(api_version_);}
        }
    ));
    connections_.insert(std::make_pair
    (
        HandlerType::kLogin
        ,new Tools::HandlerConnection
        {
            CPW::Tools::Route(std::vector<std::string>{"api", api_version_})
            ,[&](){return new CPW::Handlers::LoginHandler(api_version_);}
        }
    ));
    connections_.insert(std::make_pair
    (
        HandlerType::kFrontend
        ,new Tools::HandlerConnection
        {
            CPW::Tools::Route(std::vector<std::string>{""})
            ,[&](){return new CPW::Handlers::FrontendHandler(api_version_);}
        }
    ));
}
