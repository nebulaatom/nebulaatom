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

#ifndef CPW_HANDLERS_ROOTHANDLER_H
#define CPW_HANDLERS_ROOTHANDLER_H


#include <istream>
#include <string>
#include <list>
#include <map>
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
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>

#include "core/query_actions.h"
#include "tools/route.h"
#include "tools/common_responses.h"
#include "extras/security_verification.h"
#include "extras/dynamic_elements.h"
#include "extras/http_methods.h"


namespace CPW
{
	namespace Handlers
	{
		class RootHandler;
	}
}

using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;
using namespace Poco::Data::Keywords;

using Poco::Data::Session;
using Poco::Data::Statement;


class CPW::Handlers::RootHandler :
	public HTTPRequestHandler
	,public CPW::Extras::SecurityVerification
{
	public:
		RootHandler(std::string api_version);
		virtual ~RootHandler();
		virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

		std::string get_api_verion() const {return api_verion_;}
		Application& get_app() const {return app_;};

	protected:
		virtual void AddRoutes_() = 0;
		bool IdentifyRoute_();
		bool ManageRequestBody_(HTTPServerRequest& request);

		Application& app_;

	private:
		std::string api_verion_;
		bool route_verification_;
		std::map<std::string, std::pair<CPW::TypeAction, std::function<void(HTTPServerRequest&, HTTPServerResponse&)>>> actions_strings_;
};

#endif // CPW_HANDLERS_ROOTHANDLER_H
