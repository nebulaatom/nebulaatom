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

#include "handler_factory.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "handlers/null_handler.h"
#include "http/common_responses.h"
#include "tools/handler_connection.h"

using namespace Atom::Core;

HandlerFactory::HandlerFactory()
{
    set_request_type(HTTP::RequestType::kConstRequest);
    handler_creator_ = [&](HTTP::Request& request)
    {
        // Find route and handler
        FunctionHandler f;

        if(!request.get_http_server_const_request().has_value())
        {
            f = [&](){return new Atom::Handlers::NullHandler();};
            return f();
        }
        
        auto found = connections_.find(request.get_http_server_const_request().value()->getURI());
        if(found != connections_.end())
            f = found->second.return_handler_;
        else
            f = [&](){return new Atom::Handlers::NullHandler();};

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
        // Set request and response
            SetupConstRequest_(request);
            SetupResponse_(request.response());

        // Return handler
            return handler_creator_(*this);
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::instance_.Log_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error with the database. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        Tools::OutputLogger::instance_.Log_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(Poco::NullPointerException& error)
    {
        Tools::OutputLogger::instance_.Log_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::instance_.Log_("Error on handler_factory.cpp on createRequestHandler(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::instance_.Log_("Error on handler_factory.cpp on createRequestHandler(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }

    return new Atom::Handlers::NullHandler();
}
