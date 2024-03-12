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

#include "nebula_atom.h"
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/NetSSL.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Util/Application.h>

using namespace Atom::Core;

NebulaAtom::NebulaAtom(bool use_ssl) :
    use_ssl_(use_ssl)
    ,server_params_(new HTTPServerParams())
    ,handler_factory_(new HandlerFactory())
    ,app_(Application::instance())
{
    Tools::SettingsManager::SetUpProperties_();
    if(use_ssl_)
        Net::initializeSSL();
}

NebulaAtom::~NebulaAtom()
{
    if(use_ssl_)
        Net::uninitializeSSL();
}

void NebulaAtom::CustomHandlerCreator_(HandlerFactory::FunctionHandlerCreator handler_creator)
{
    handler_factory_->set_handler_creator(std::move(handler_creator));
}

void NebulaAtom::AddHandler_(std::string route, HandlerFactory::FunctionHandler handler)
{
    std::vector<std::string> segments;
    URI(route).getPathSegments(segments);
    Tools::Route requested_route(segments);

    handler_factory_->get_connections().insert({route, Tools::HandlerConnection{requested_route, handler}});
}

void NebulaAtom::initialize(Application& self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

void NebulaAtom::uninitialize()
{
    ServerApplication::uninitialize();
}

int NebulaAtom::main(const std::vector<std::string>& args)
{
    try
    {
        arguments_ = &args;
        server_params_->setMaxQueued(settings_manager_.get_basic_properties_().max_queued);
        server_params_->setMaxThreads(settings_manager_.get_basic_properties_().max_threads);

        if(use_ssl_)
        {
            Context::Ptr context = new Context
            (
                Context::SERVER_USE
                ,settings_manager_.get_basic_properties_().key
                ,settings_manager_.get_basic_properties_().certificate
                ,settings_manager_.get_basic_properties_().rootcert
                ,Context::VERIFY_RELAXED
                ,9
                ,false
                ,"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
            );
            secure_server_socket_ = std::make_shared<SecureServerSocket>(settings_manager_.get_basic_properties_().port, 64, context);
            
            server_ = std::make_unique<HTTPServer>(handler_factory_, *secure_server_socket_.get(), server_params_);
        }
        else
        {
            server_socket_ = std::make_shared<ServerSocket>(settings_manager_.get_basic_properties_().port);
            server_ = std::make_unique<HTTPServer>(handler_factory_, *server_socket_.get(), server_params_);
        }
        return Init_();
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(Poco::NullPointerException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(Poco::SystemException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(Net::SSLException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
        return -1;
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
        return -1;
    }

    return -1;
}

int NebulaAtom::Init_()
{
    server_->start();
    app_.logger().information("- Server started at port " + format("%d", settings_manager_.get_basic_properties_().port));

    waitForTerminationRequest();

    app_.logger().information("- Shutting down server... ");
    server_->stop();

    return Application::EXIT_OK;
}
