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

#ifndef ATOMANDLERS_ROOTHANDLER
#define ATOMANDLERS_ROOTHANDLER


#include <istream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>

#include "yaml-cpp/yaml.h"
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

#include "tools/sessions_manager.h"
#include "query/database_manager.h"
#include "query/query_actions.h"
#include "tools/route.h"
#include "http/common_responses.h"
#include "security/security_verification.h"
#include "http/http_methods.h"
#include "tools/requests_manager.h"
#include "tools/manage_json.h"
#include "security/user.h"
#include "functions/functions_manager.h"
#include "query/condition.h"
#include "tools/settings_manager.h"


namespace Atom
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


class AtomHandlers::RootHandler :
    public HTTPRequestHandler
    ,public HTTP::CommonResponses
    ,public HTTP::HTTPMethods
    ,public Tools::ManageJSON
{
    public:
        RootHandler();
        virtual ~RootHandler();

        std::string get_user() const { return user_; }
        std::string get_method() const { return method_; }
        bool get_route_verification() const { return route_verification_; }
        Application& get_app() const {return app_;}
        Tools::SettingsManager& get_settings_manager()
        {
            auto& var = settings_manager_;
            return var;
        }
        Extras::SecurityVerification& get_current_security()
        {
            auto& var = current_security_;
            return var;
        }
        std::list<Tools::Route>& get_routes_list()
        {
            auto& var = routes_list_;
            return var;
        }
        std::shared_ptr<AtomTools::Route>& get_requested_route()
        {
            auto& var = requested_route_;
            return var;
        }
        Functions::FunctionsManager& get_functions_manager()
        {
            auto& var = functions_manager_;
            return var;
        }
        Functions::Function& get_current_function()
        {
            auto& var = current_function_;
            return var;
        }

        virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

    protected:
        HTTPServerRequest* get_request() { return request_; }
        HTTPServerResponse* get_response() { return response_; }
        
        virtual void AddFunctions_();
        virtual void Process_();
        void CallHTTPMethod_();
        bool ProcessRoute_();
        bool VerifySession_();
        bool VerifyPermissions_();
        bool IdentifyRoute_();
        void ManageRequestBody_();
        virtual void HandlePOSTMethod_();
        virtual void HandleGETMethod_();
        virtual void HandlePUTMethod_();
        virtual void HandleDELMethod_();

        Application& app_;

    private:
        std::string user_;
        std::string method_;
        bool route_verification_;
        Tools::SettingsManager settings_manager_;
        Extras::SecurityVerification current_security_;
        std::list<std::string> targets_;
        HTTPServerRequest* request_;
        HTTPServerResponse* response_;
        std::list<Tools::Route> routes_list_;
        std::shared_ptr<AtomTools::Route> requested_route_;
        Functions::FunctionsManager functions_manager_;
        Functions::Function current_function_;
};

#endif // ATOMANDLERS_ROOTHANDLER
