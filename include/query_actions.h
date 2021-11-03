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

#ifndef CPW_QUERYACTIONS_H
#define CPW_QUERYACTIONS_H


#include <string>
#include <map>

#include <Poco/Net/HTTPServerRequest.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>
#include <Poco/URI.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/RecordSet.h>


namespace CPW
{
	enum class TypeAction;
	class Filters;
	class QueryActions;
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Data::Keywords;



enum class CPW::TypeAction
{
	kInsert
	,kSelect
	,kUpdate
	,kDelete
};

class CPW::Filters
{
	public:
		Filters();
		~Filters();

		std::string get_fields() const { return fields_; }
		std::string get_page() const { return page_; }
		std::string get_limit() const { return limit_; }
		std::string get_sorts_conditions() const { return sorts_conditions_; }
		std::map<std::string, std::string>& get_iquals_conditions()
		{
			auto& ic = iquals_conditions_;
			return ic;
		}

		void set_fields(std::string fields) { fields_ = fields; }
		void set_page(std::string page) { page_ = page; }
		void set_limit(std::string limit) { limit_ = limit; }
		void set_sorts_conditions(std::string sorts_conditions) { sorts_conditions_ = sorts_conditions; }

	private:
		std::string fields_;
		std::string page_;
		std::string limit_;
		std::string sorts_conditions_;
		std::map<std::string, std::string> iquals_conditions_;
};


class CPW::QueryActions
{
	public:
		QueryActions();
		~QueryActions();

		std::string get_final_query() const {return final_query_;}
		Filters get_current_filters_() const {return current_filters_;}
		Data::Session get_session() const {return session_;}
		Data::Statement get_query() const {return query_;}
		std::map<std::string, std::string>* get_table_rows() const {return table_rows_;}
		Poco::JSON::Array* get_result_json() const {return result_json_;}

		void ResetQuery_();
		void IdentifyFilters_(HTTPServerRequest& request);
		void ComposeQuery_(TypeAction action_type, std::string table, std::string body);
		void ExecuteQuery_();

	protected:
		void CreateRows_(TypeAction action_type);
		std::string GetSortsConditions_();
		std::string GetIqualsConditions_();
		std::string ComposeInsertSentence_(std::string table, std::string body);
		std::string ComposeSelectSentence_(std::string table);
		std::string ComposeUpdateSentence_(std::string table, std::string body);
		std::string ComposeDeleteSentence_(std::string table, std::string body);

	private:
		std::string final_query_;
		Filters current_filters_;
		Data::Session session_;
		Data::Statement query_;
		std::map<std::string, std::string>* table_rows_;
		Poco::JSON::Array* result_json_;
};


#endif // CPW_QUERYACTIONS_H
