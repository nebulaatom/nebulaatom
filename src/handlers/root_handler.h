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

#ifndef NAF_HANDLERS_ROOTHANDLER
#define NAF_HANDLERS_ROOTHANDLER


#include <istream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>

#include "functions/action.h"
#include "yaml-cpp/yaml.h"
#include "Poco/Exception.h"
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

#include "nebulaatomConfig.h"
#include "tools/sessions_manager.h"
#include "query/database_manager.h"
#include "tools/route.h"
#include "http/common_responses.h"
#include "security/security_verification.h" 
#include "tools/manage_json.h"
#include "security/user.h"
#include "functions/functions_manager.h"
#include "query/condition.h"
#include "tools/settings_manager.h"
#include "tools/output_logger.h"
#include "http/methods.h"
#include "http/body.h"


namespace NAF
{
    namespace Handlers
    {
        class ReferenceContainer;
        class RootHandler;
    }
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Data::Keywords;

using Poco::Data::Session;
using Poco::Data::Statement;


class NAF::Handlers::RootHandler :
    public HTTPRequestHandler
    ,public HTTP::CommonResponses
    ,public HTTP::Methods
    ,public Extras::SecurityVerification
    ,public HTTP::Body
{
    public:
        struct Properties
        {
            std::string method;
            std::string uri;
            std::string content_type;
            Net::NameValueCollection content_type_parameters;
        };

        RootHandler();
        virtual ~RootHandler();

        struct Properties& get_properties()
        {
            auto& var = properties_;
            return var;
        }
        std::shared_ptr<NAF::Tools::Route>& get_requested_route()
        {
            auto& var = requested_route_;
            return var;
        }
        Functions::FunctionsManager& get_functions_manager()
        {
            auto& var = functions_manager_;
            return var;
        }
        Functions::Function::Ptr& get_current_function()
        {
            auto& var = current_function_;
            return var;
        }

        virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
        bool SetupSSL_();
        Functions::Function::Ptr AddFunction_(std::string endpoint, HTTP::EnumMethods method);
        virtual void Process_() = 0;
        bool VerifySession_();
        bool VerifyPermissions_();
        bool IdentifyRoute_();
        void ManageRequestBody_();

    protected:
        void SetupProperties_();
        void IdentifyParameters_(Functions::Action::Ptr action);
        void IdentifyParameters_();

    private:
        struct Properties properties_;
        std::list<std::string> targets_;
        std::shared_ptr<NAF::Tools::Route> requested_route_;
        Functions::FunctionsManager functions_manager_;
        Functions::Function::Ptr current_function_;
};

#endif // NAF_HANDLERS_ROOTHANDLER
