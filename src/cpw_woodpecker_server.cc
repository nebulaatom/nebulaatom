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
	svs(port_)
{

}

WoodpeckerServer::~WoodpeckerServer()
{
	//dtor
}

int WoodpeckerServer::main(const std::vector<std::string>& args)
{
	pParams = new HTTPServerParams();
	pParams->setMaxQueued(100);
	pParams->setMaxThreads(16);

	std::unique_ptr<HTTPServer> srv_test(new HTTPServer(new CPW::HandlerFactory(), svs, pParams));
	srv = std::move(srv_test);

	return Init_();
}

