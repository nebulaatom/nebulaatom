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

#ifndef CPW_FACTORY_ROOTHANDLER_H
#define CPW_FACTORY_ROOTHANDLER_H


#include <iostream>
#include <string>
#include <set>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>
#include <Poco/Format.h>
#include <Poco/NumberFormatter.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>


namespace CPW
{
	namespace Factory
	{
		class RootHandler;
	}
}

using namespace Poco::Data::Keywords;
using namespace Poco;
using namespace Poco::Net;

using Poco::Data::Session;
using Poco::Data::Statement;


class CPW::Factory::RootHandler : public HTTPRequestHandler
{
	public:
		struct User
		{
			std::string user, password;
		};

		RootHandler();
		virtual ~RootHandler();

	private:
		std::string current_route_;
		std::set<std::string> routes_list_;
};

#endif // CPW_FACTORY_ROOTHANDLER_H
