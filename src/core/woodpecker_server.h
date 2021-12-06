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

#ifndef CPW_CORE_WOODPECKERSERVER_H
#define CPW_CORE_WOODPECKERSERVER_H


#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include "Poco/Format.h"

#include "core/handler_factory.h"


namespace CPW
{
	namespace Core
	{
		class WoodpeckerServer;
	}
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


class CPW::Core::WoodpeckerServer : public ServerApplication
{
	public:
		WoodpeckerServer(int port);
		virtual ~WoodpeckerServer();

	protected:
		virtual int main(const std::vector<std::string>& args);
		int Init_();

	private:
		Poco::UInt16 port_;
		Poco::Net::HTTPServerParams* server_params_;
		Poco::Net::ServerSocket server_socket_;
		std::unique_ptr<HTTPServer> server_;
		HandlerFactory* handler_factory_;
		Application& app_;
};

#endif // CPW_CORE_WOODPECKERSERVER_H
