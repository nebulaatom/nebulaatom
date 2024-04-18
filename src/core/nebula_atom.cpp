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

#if defined(POCO_OS_FAMILY_WINDOWS)
	NamedEvent terminator(ProcessImpl::terminationEventName(Process::id()));
#else
	Poco::Event terminator;
#endif

using namespace Atom::Core;

NebulaAtom::NebulaAtom(bool use_ssl) :
    use_ssl_(use_ssl)
    ,handler_factory_(new HandlerFactory())
{
    Tools::SettingsManager::SetUpProperties_();
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

int NebulaAtom::Init_()
{
    if(!SettingUpServer_())
        return -1;

    server_->start();
    Tools::OutputLogger::Log_("Server started at port " + std::to_string(Tools::SettingsManager::get_basic_properties_().port));

    terminator.wait();

    Tools::OutputLogger::Log_("Shutting down server... ");
    server_->stop();

    return 0;
}

int NebulaAtom::Init_(int argc, char** argv)
{
    // Setting up console parameters
    console_parameters_ = std::vector<std::string>(argv, argv + argc);

    if(!SettingUpServer_())
        return -1;

    server_->start();
    Tools::OutputLogger::Log_("Server started at port " + std::to_string(Tools::SettingsManager::get_basic_properties_().port));

    terminator.wait();

    Tools::OutputLogger::Log_("Shutting down server... ");
    server_->stop();

    return 0;
}

int NebulaAtom::SettingUpServer_()
{
    try
    {
        auto server_params = new HTTPServerParams;
        server_params->setTimeout(Tools::SettingsManager::get_basic_properties_().timeout);
        server_params->setMaxThreads(Tools::SettingsManager::get_basic_properties_().max_threads);
        server_params->setMaxQueued(Tools::SettingsManager::get_basic_properties_().max_queued);

        if(use_ssl_)
        {
            if (Tools::SettingsManager::get_basic_properties_().key.empty() && Tools::SettingsManager::get_basic_properties_().certificate.empty())
                throw SSLException("Configuration error: no certificate file has been specified");

            // Setup SSL
            Net::initializeSSL();
            Net::Context::Ptr context = new Net::Context
            (
                Net::Context::TLS_SERVER_USE
                ,Tools::SettingsManager::get_basic_properties_().key
                ,Tools::SettingsManager::get_basic_properties_().certificate
                ,Tools::SettingsManager::get_basic_properties_().rootcert
                ,Net::Context::VERIFY_RELAXED
            );
            SharedPtr<Net::InvalidCertificateHandler> cert_handler = new Net::AcceptCertificateHandler(true);
            SharedPtr<Net::PrivateKeyPassphraseHandler> keyhandler = new Net::KeyConsoleHandler(true);

            Net::SSLManager::instance().initializeServer(keyhandler, cert_handler, context);

            // SSL Server
            SecureServerSocket secure_server_socket(Tools::SettingsManager::get_basic_properties_().port);
            server_ = std::make_unique<HTTPServer>(handler_factory_, secure_server_socket, server_params);
        }
        else
        {
            ServerSocket server_socket(Tools::SettingsManager::get_basic_properties_().port);
            server_ = std::make_unique<HTTPServer>(handler_factory_, server_socket, server_params);
        }

        return true;
    }
    catch(Net::SSLException& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + error.displayText());
        return false;
    }
    catch(Poco::NullPointerException& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + error.displayText());
        return false;
    }
    catch(Poco::SystemException& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + error.displayText());
        return false;
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + std::string(error.what()));
        return false;
    }
    catch(...)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): Unhandled");
        return false;
    }

    return false;
}