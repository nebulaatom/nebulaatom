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


namespace CPW
{
    namespace Query
    {
        enum class TypeAction;
        class QueryActions;
    }
}

using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;


enum class CPW::Query::TypeAction
{
    kInsert
    ,kSelect
    ,kUpdate
    ,kDelete
};


class CPW::Query::QueryActions :
    public Tools::ManageJSON
    ,public HTTP::CommonResponses
{
    public:
        QueryActions();
        ~QueryActions();

        std::string get_final_query() const {return final_query_;}
        int get_affected_rows_() const {return affected_rows_;}
        std::shared_ptr<Filters::FiltersManager>& get_current_filters_()
        {
            auto& var = current_filters_;
            return var;
        }
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
        JSON::Object::Ptr get_result_json() const {return result_json_;}

        void IdentifyFilters_();
        void ResetFilters_();
        bool ComposeQuery_(TypeAction action_type, std::string table);
        bool ExecuteQuery_(HTTPServerResponse& response);
        bool ExecuteQuery_();
        bool CreateJSONResult_();

    protected:
        std::string ComposeInsertSentence_(std::string table);
        std::string ComposeSelectSentence_(std::string table);
        std::string ComposeUpdateSentence_(std::string table);
        std::string ComposeDeleteSentence_(std::string table);
        std::string MakeFinalQuery_(std::vector<std::string>& tmp_query);

    private:
        std::string final_query_;
        int affected_rows_;
        std::shared_ptr<Filters::FiltersManager> current_filters_;
        std::shared_ptr<Data::Session> session_;
        std::shared_ptr<Data::Statement> query_;
        JSON::Object::Ptr result_json_;
        Application& app_;
        std::shared_ptr<Tools::RowValueFormatter> row_value_formatter_;
};


#endif // CPW_QUERY_QUERYACTIONS_H
