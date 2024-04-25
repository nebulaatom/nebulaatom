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

HandlerFactory::HandlerFactory() :
    handler_creator_(nullptr)
{

}

HandlerFactory::~HandlerFactory()
{

}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    try
    {
        if(handler_creator_ == nullptr)
        {
            handler_creator_ = [&](Core::BasicInfo& info)
            {
                // Find route and handler
                FunctionHandler f;

                auto found = connections_.find(info.uri);
                if(found != connections_.end())
                    f = found->second.return_handler_;
                else
                    f = [&](){return new Atom::Handlers::NullHandler();};

                return f();
            };
        }
        else
        {
            Core::BasicInfo info(request.getURI());
            return handler_creator_(info);
        }
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::Log_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        ErrorResponse_(request, "Error with the database. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        Tools::OutputLogger::Log_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        ErrorResponse_(request, "Internal server error. " + error.displayText());
    }
    catch(Poco::NullPointerException& error)
    {
        Tools::OutputLogger::Log_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        ErrorResponse_(request, "Internal server error. " + error.displayText());
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Log_("Error on handler_factory.cpp on createRequestHandler(): " + std::string(error.what()));
        ErrorResponse_(request, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Log_("Error on handler_factory.cpp on createRequestHandler(): " + std::string(error.what()));
        ErrorResponse_(request, "Internal server error. " +  std::string(error.what()));
    }

    return new Atom::Handlers::NullHandler();
}

void HandlerFactory::ErrorResponse_(const HTTPServerRequest& request, std::string error)
{
    request.response().setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    request.response().setContentType("text/html");
    request.response().setChunkedTransferEncoding(true);

    std::ostream& out = request.response().send();

    out << "<html>";
    out << "<head><title> 500 HTTP_INTERNAL_SERVER_ERROR | Nebula Atom</title></head>";
    out << "<body>";
    out << "<center><h1>Status: 500 HTTP_INTERNAL_SERVER_ERROR </h1></center>";
    out << "<center><h3>Message: " << error << "</h3></center>";
    out << "<center><hr>Nebula Atom/" << PACKAGE_VERSION_COMPLETE << "</center>";
    out << "</body>";
    out << "</html>";
}