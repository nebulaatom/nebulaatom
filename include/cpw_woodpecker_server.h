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

#ifndef CPWWOODPECKERSERVER_H
#define CPWWOODPECKERSERVER_H


#include <string>
#include <vector>
#include <iostream>
#include <memory>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>

#include "handler_factory.h"


using namespace Poco::Net;
using namespace Poco::Util;


namespace CPW
{
	class WoodpeckerServer;
}


class WoodpeckerServer : public ServerApplication
{
	public:
		WoodpeckerServer();
		virtual ~WoodpeckerServer();
};

#endif // CPWWOODPECKERSERVER_H
