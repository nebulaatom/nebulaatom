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
#include <list>
#include <vector>
#include <algorithm>

#include "Poco/Util/ServerApplication.h"
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

#include "query_actions.h"
#include "route.h"
#include "error_report.h"



namespace CPW
{
	namespace Factory
	{
		class RootHandler;
	}
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Data::Keywords;

using Poco::Data::Session;
using Poco::Data::Statement;


class CPW::Factory::RootHandler : public HTTPRequestHandler, public ErrorReport
{
	public:
		struct User
		{
			std::string user, password;
		};

		RootHandler(std::string api_version);
		virtual ~RootHandler();
		virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

		std::string get_api_verion() const {return api_verion_;}
		std::list<Route*>* get_routes_list() const {return routes_list_;}
		QueryActions* get_current_query_actions() const {return current_query_actions_;}
		Poco::DynamicStruct get_dynamic_json_body() const {return dynamic_json_body_;}

	protected:
		void ReadJSONBody_(HTTPServerRequest& request);
		bool SecurityVerification_(HTTPServerRequest& request, HTTPServerResponse& response);
		bool AuthenticateUser_();
		bool VerifyPermissions_(HTTPServerRequest& request);
		void SeePermissionsPerUser_(std::string user, std::string action_type, std::string target);
		virtual void HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response) = 0;
		virtual void HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response) = 0;
		virtual void HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response) = 0;
		virtual void HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response) = 0;
		virtual void AddRoutes_() = 0;
		bool IdentifyRoute_(HTTPServerRequest& request);

	private:
		std::string api_verion_;
		bool route_verification_;
		std::unique_ptr<Route> requested_route_;
		std::list<Route*>* routes_list_;
		QueryActions* current_query_actions_;
		Poco::DynamicStruct dynamic_json_body_;
};

#endif // CPW_FACTORY_ROOTHANDLER_H
