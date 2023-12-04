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

#ifndef CPW_QUERY_QUERYACTIONS_H
#define CPW_QUERY_QUERYACTIONS_H


#include <istream>
#include <string>
#include <array>
#include <map>
#include <stdexcept>

#include "Poco/Util/Application.h"
#include <Poco/Net/HTTPServerRequest.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/RecordSet.h>

#include "query/database_manager.h"
#include "tools/manage_json.h"
#include "filters/filters_manager.h"
#include "http/common_responses.h"
#include "tools/row_value_formatter.h"
#include "functions/function.h"
#include "query/results.h"


namespace CPW
{
    namespace Query
    {
        class QueryActions;
    }
}

using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;


class CPW::Query::QueryActions :
    public Tools::ManageJSON
    ,public HTTP::CommonResponses
{
    public:
        QueryActions();
        ~QueryActions();

        std::string get_final_query() const {return final_query_;}
        int get_affected_rows_() const {return affected_rows_;}
        std::shared_ptr<Data::Session>& get_session()
        {
            auto& var = session_;
            return var;
        }
        std::shared_ptr<Data::Statement>& get_query()
        {
            auto& var = query_;
            return var;
        }

        void IdentifyParameters_(Functions::Action& action);
        bool ComposeQuery_(Functions::Action& action);
        void ExecuteQuery_(Functions::Action& action);
        Query::Results MakeResults_(Functions::Action& action);
        JSON::Object::Ptr CreateJSONResult_();

    private:
        void StablishActionError_(Functions::Action& action, std::string error);

        std::string final_query_;
        int affected_rows_;
        std::shared_ptr<Data::Session> session_;
        std::shared_ptr<Data::Statement> query_;
        Application& app_;
};


#endif // CPW_QUERY_QUERYACTIONS_H
