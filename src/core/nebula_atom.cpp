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

using namespace Atom::Core;

NebulaAtom::NebulaAtom(bool use_ssl) :
    use_ssl_(use_ssl)
    ,handler_factory_(new HandlerFactory())
{
    if(use_ssl_)
        Net::initializeSSL();

    Tools::SettingsManager::SetUpProperties_();
}

NebulaAtom::~NebulaAtom()
{
    if(use_ssl_)
        Net::uninitializeSSL();
}

void NebulaAtom::initialize(Application& self)
{
    ServerApplication::initialize(self);
}
    
void NebulaAtom::uninitialize()
{
    ServerApplication::uninitialize();
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
    return run();
}

int NebulaAtom::Init_(int argc, char** argv)
{
    return run(argc, argv);
}

int NebulaAtom::main(const std::vector<std::string >& args)
{
    try
    {
        // Setting up console parameters
        console_parameters_ = std::vector<std::string>(args);

        // Setup server
        SetupServer_();

        // Start server, wait and stop
        server_->start();
        Tools::OutputLogger::Log_("Server started at port " + std::to_string(Tools::SettingsManager::get_basic_properties_().port));

        waitForTerminationRequest();

        Tools::OutputLogger::Log_("Shutting down server... ");
        server_->stop();
        
        return 0;
    }
    catch(Net::SSLException& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + error.displayText());
        return 1;
    }
    catch(Poco::NullPointerException& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + error.displayText());
        return 1;
    }
    catch(Poco::SystemException& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + error.displayText());
        return 1;
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + std::string(error.what()));
        return 1;
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): " + std::string(error.what()));
        return 1;
    }
    catch(...)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on main(): Unhandled");
        return 1;
    }
}

void NebulaAtom::SetupServer_()
{
    try
    {
        // Setup Socket and Server
        if(use_ssl_)
        {
            SetupSSL_();

            SecureServerSocket secure_server_socket(Tools::SettingsManager::get_basic_properties_().port);
            server_ = std::make_shared<Core::Server>(handler_factory_, secure_server_socket, new HTTPServerParams);
        }
        else
        {
            ServerSocket server_socket(Tools::SettingsManager::get_basic_properties_().port);
            server_ = std::make_shared<Core::Server>(handler_factory_, server_socket, new HTTPServerParams);
        }
        }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on SetupServer_(): " + std::string(error.what()));
        return;
    }
}

void NebulaAtom::SetupSSL_()
{
    try
    {
        // Setup certificate handler and key handler
        SharedPtr<Net::InvalidCertificateHandler> cert_handler = new Net::ConsoleCertificateHandler(true);
        SharedPtr<Net::PrivateKeyPassphraseHandler> keyhandler = new Net::KeyConsoleHandler(true);

        Context::Params params;
        params.privateKeyFile = Tools::SettingsManager::get_basic_properties_().key;
        params.certificateFile = Tools::SettingsManager::get_basic_properties_().certificate;
        params.caLocation = Tools::SettingsManager::get_basic_properties_().rootcert;

        if (params.certificateFile.empty() && params.privateKeyFile.empty())
            throw SSLException("Configuration error: no certificate file has been specified");

        // optional options for which we have defaults defined
        params.verificationMode = Context::VERIFY_RELAXED;
        params.verificationDepth = 9;
        params.loadDefaultCAs = true;
        params.cipherList = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH";
        params.dhParamsFile = "";
        params.ecdhCurve = "";

        Context::Usage usage = Context::TLSV1_3_SERVER_USE;
        context_ = new Context(usage, params);

        context_->enableSessionCache(false);
        context_->enableExtendedCertificateVerification(false);
        context_->preferServerCiphers();

        Net::SSLManager::instance().initializeServer(keyhandler, cert_handler, context_);
    }
    catch(Net::SSLException& error)
    {
        Tools::OutputLogger::Log_("Error on nebula_atom.cpp on SetupSSL_(): " + error.displayText());
        return;
    }
}
