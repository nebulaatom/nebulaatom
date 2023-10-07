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
#include <functional>
#include <stdexcept>

#include "Poco/Exception.h"
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
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/X509Certificate.h"

#include "tools/sessions_handler.h"
#include "query/database_manager.h"
#include "tools/route.h"
#include "http/common_responses.h"
#include "security/security_verification.h"
#include "extras/dynamic_elements.h"
#include "http/http_methods.h"
#include "tools/requests_manager.h"
#include "tools/manage_json.h"
#include "security/user.h"
#include "functions/functions_manager.h"


namespace CPW
{
    namespace Handlers
    {
        class ReferenceContainer;
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
    ,public HTTP::CommonResponses
    //,public HTTP::HTTPMethods
{
    public:
        RootHandler(std::string api_version);
        virtual ~RootHandler();

        std::string get_api_version() const {return api_version_;}
        Application& get_app() const {return app_;};
        Extras::SecurityVerification& get_current_security()
        {
            auto& var = current_security_;
            return var;
        }
        Functions::FunctionsManager& get_functions_manager()
        {
            auto& var = functions_manager_;
            return var;
        }
        HTTPServerRequest* get_request() const {return request_;}
        HTTPServerResponse* get_response() const {return response_;}
        std::list<Tools::Route>& get_routes_list()
        {
            auto& var = routes_list_;
            return var;
        }
        std::shared_ptr<CPW::Tools::Route>& get_requested_route()
        {
            auto& var = requested_route_;
            return var;
        }
        std::shared_ptr<Query::QueryActions>& get_query_actions()
        {
            auto& var = query_actions_;
            return var;
        }

        void set_request(HTTPServerRequest& request) {request_ = &request;}
        void set_response(HTTPServerResponse& response) {response_ = &response;}

        virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

    protected:
        virtual void AddRoutes_() = 0;
        bool ProcessRoute_();
        bool VerifySession_();
        bool VerifyPermissions_();
        bool IdentifyRoute_();
        bool ManageRequestBody_();

        Application& app_;

    private:
        std::string api_version_;
        std::string user_;
        std::string method_;
        bool route_verification_;
        Extras::SecurityVerification current_security_;
        //Tools::RequestsManager requests_manager_;
        //std::shared_ptr<Extras::DynamicElements> dynamic_elements_;
        std::list<std::string> targets_;
        Functions::FunctionsManager functions_manager_;
        HTTPServerRequest* request_;
        HTTPServerResponse* response_;
        std::list<Tools::Route> routes_list_;
        std::shared_ptr<CPW::Tools::Route> requested_route_;
        std::shared_ptr<Query::QueryActions> query_actions_;
        Functions::Function* current_function_;
};

#endif // CPW_HANDLERS_ROOTHANDLER_H
