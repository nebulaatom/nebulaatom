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

#include "cpw_woodpecker_server.h"

using namespace CPW;

WoodpeckerServer::WoodpeckerServer(int port) :
	port_(port),
	server_socket_(port_)
{
	handler_factory_ = new HandlerFactory();
}

WoodpeckerServer::~WoodpeckerServer()
{
	delete handler_factory_;
}

int WoodpeckerServer::main(const std::vector<std::string>& args)
{
	server_params_ = new HTTPServerParams();
	server_params_->setMaxQueued(100);
	server_params_->setMaxThreads(16);

	std::unique_ptr<HTTPServer> srv_test(new HTTPServer(handler_factory_, server_socket_, server_params_));
	server_ = std::move(srv_test);

	return Init_();
}

int WoodpeckerServer::Init_()
{
	server_->start();
	std::cout << std::endl << "Server started at port " << port_ << std::endl;

	waitForTerminationRequest();

	std::cout << std::endl << "Shutting down server..." << std::endl;
	server_->stop();

	return Application::EXIT_OK;
}
