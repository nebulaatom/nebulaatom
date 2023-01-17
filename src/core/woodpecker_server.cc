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

#include "woodpecker_server.h"

using namespace CPW::Core;

WoodpeckerServer::WoodpeckerServer(Poco::UInt16 port) :
    port_(port)
    ,server_params_(new HTTPServerParams())
    ,handler_factory_(new HandlerFactory())
    ,app_(Application::instance())
{
    Poco::Net::initializeSSL();
}

WoodpeckerServer::~WoodpeckerServer()
{
    Poco::Net::uninitializeSSL();
}

void WoodpeckerServer::initialize(Application& self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

void WoodpeckerServer::uninitialize()
{
    ServerApplication::uninitialize();
}

int WoodpeckerServer::main(const std::vector<std::string>& args)
{
    arguments_ = &args;
    server_params_->setMaxQueued(100);
    server_params_->setMaxThreads(16);

    server_socket_ = std::make_shared<SecureServerSocket>(port_);
    server_ = std::make_unique<HTTPServer>(handler_factory_, *server_socket_.get(), server_params_);

    return Init_();
}

int WoodpeckerServer::Init_()
{
    server_->start();
    app_.logger().information("- Server started at port " + format("%d", static_cast<int>(port_)));

    waitForTerminationRequest();

    app_.logger().information("- Shutting down server... ");
    server_->stop();

    return Application::EXIT_OK;
}
