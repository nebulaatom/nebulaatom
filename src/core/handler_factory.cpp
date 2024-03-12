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
#include "Poco/Net/HTTPServerResponse.h"
#include "handlers/null_handler.h"
#include "http/common_responses.h"
#include "tools/handler_connection.h"

using namespace AtomCore;

HandlerFactory::HandlerFactory() :
    app_(Application::instance())
{
    handler_creator_ = [&](const HTTPServerRequest& request)
    {
        // Find route and handler
        FunctionHandler f;
        auto found = connections_.find(request.getURI());
        if(found != connections_.end())
            f = found->second.return_handler_;
        else
            f = [&](){return new AtomHandlers::NullHandler();};

        return f();
    };
}

HandlerFactory::~HandlerFactory()
{

}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    try
    {
        HTTP::CommonResponses::set_response(&request.response());

        return handler_creator_(request);
        
        /*std::vector<std::string> segments;

        URI(request.getURI()).getPathSegments(segments);

        AtomTools::Route requested_route(segments);

        switch(requested_route.get_current_route_type())
        {
            case AtomTools::RouteType::kEndpoint:
            {
                std::vector<std::string> login_route({"api", api_version_, "system", "login"});
                std::vector<std::string> logout_route({"api", api_version_, "system", "logout"});

                if
                (
                    requested_route.get_segments() == login_route
                    || requested_route.get_segments() == logout_route
                )
                    return handlers_[HandlerType::kLogin]->return_handler_();
                else
                    return handlers_[HandlerType::kBackend]->return_handler_();

                break;
            }
            case AtomTools::RouteType::kEntrypoint:
            {
                return handlers_[HandlerType::kFrontend]->return_handler_();
                break;
            }
        }*/
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error with the database. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(Poco::NullPointerException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }

    return new AtomHandlers::NullHandler();
}
