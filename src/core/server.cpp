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

#include "core/server.h"
#include <memory>

using namespace NAF::Core;

Server::Server(HandlerFactory* handler_factory) :
    handler_factory_(handler_factory)
{
    if(handler_factory_ == nullptr)
        handler_factory_ = new HandlerFactory();

}

void Server::SetupParams_(HTTPServerParams::Ptr params)
{
    params->setServerName(server_name_);
    params->setMaxQueued(Tools::SettingsManager::get_basic_properties_().max_queued);
    params->setMaxThreads(Tools::SettingsManager::get_basic_properties_().max_threads);
}

void Server::initialize(Application& self)
{
    ServerApplication::initialize(self);

    if(use_ssl_)
    {
        auto& config = self.config();
        config.setString("openSSL.server.privateKeyFile", Tools::SettingsManager::get_basic_properties_().key);
        config.setString("openSSL.server.certificateFile", Tools::SettingsManager::get_basic_properties_().certificate);
        config.setString("openSSL.server.caConfig", Tools::SettingsManager::get_basic_properties_().rootcert);
    }
}
    
void Server::uninitialize()
{
    ServerApplication::uninitialize();
}

int Server::main(const std::vector<std::string>&)
{
    auto server_params = new HTTPServerParams;
    SetupParams_(server_params);

    // Setup Socket and Server
    if(use_ssl_)
    {
        SecureServerSocket secure_server_socket(Tools::SettingsManager::get_basic_properties_().port);
        http_server_ = std::make_unique<HTTPServer>(handler_factory_, secure_server_socket, new HTTPServerParams);
    }
    else
    {
        ServerSocket server_socket(Tools::SettingsManager::get_basic_properties_().port);
        http_server_ = std::make_unique<HTTPServer>(handler_factory_, server_socket, new HTTPServerParams);
    }

    http_server_->start();
    Tools::OutputLogger::Log_(
        "Server started at port " + std::to_string(Tools::SettingsManager::get_basic_properties_().port)
        + " (Max threads: " + std::to_string(Tools::SettingsManager::get_basic_properties_().max_threads)
        + ", Max Queued: " + std::to_string(Tools::SettingsManager::get_basic_properties_().max_queued) + ")"
    );
    
    waitForTerminationRequest();
    
    http_server_->stop();
        
    return Application::EXIT_OK;
}