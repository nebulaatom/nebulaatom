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


#include <istream>
#include <string>
#include <map>
#include <stdexcept>

#include <Poco/Net/HTTPServerRequest.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
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
	enum class TypeQuery;
	class ManageJSON;
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

enum class CPW::TypeQuery
{
	kFields
	,kPage
	,kLimit
	,kSort
	,kIqual
	,kNotIqual
	,kGreatherThan
	,kSmallerThan
	,kBetween
	,kIn
	,kNotIn
	,kValues
};

class CPW::ManageJSON
{
	public:
		ManageJSON();
		~ManageJSON();

		Poco::DynamicStruct& get_dynamic_json_body()
		{
			Poco::DynamicStruct& d = dynamic_json_body_;
			return d;
		}
		Poco::Dynamic::Var& get_dynamic_manager()
		{
			Poco::Dynamic::Var& d = dynamic_manager_;
			return d;
		}

		std::string ReadBody_(std::istream& stream);
		void Parse_(std::string string_to_parse);

	private:
		Poco::DynamicStruct dynamic_json_body_;
		Dynamic::Var dynamic_manager_;
};

class CPW::Filters
{
	public:
		Filters();
		~Filters();

		std::vector<std::string>& get_fields()
		{
			auto& var = fields_;
			return var;
		}
		std::string get_page() const { return page_; }
		std::string get_limit() const { return limit_; }
		std::vector<std::string>& get_sorts_conditions()
		{
			auto& var = sorts_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_iquals_conditions()
		{
			auto& var = iquals_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_not_iquals_conditions()
		{
			auto& var = not_iquals_conditions_;
			return var;
		}
		std::map<std::string, std::string>& get_greather_than()
		{
			auto& var = greather_than_;
			return var;
		}
		std::map<std::string, std::string>& get_smaller_than()
		{
			auto& var = smaller_than_;
			return var;
		}
		std::map<std::string, std::string>& get_between()
		{
			auto& var = between_;
			return var;
		}
		std::map<std::string, std::vector<std::string>>& get_in()
		{
			auto& var = in_;
			return var;
		}
		std::map<std::string, std::vector<std::string>>& get_not_in()
		{
			auto& var = not_in_;
			return var;
		}
		std::vector<std::string>& get_values()
		{
			auto& var = values_;
			return var;
		}

		void set_page(std::string page) { page_ = page; }
		void set_limit(std::string limit) { limit_ = limit; }

	private:
		std::vector<std::string> fields_;
		std::string page_;
		std::string limit_;
		std::vector<std::string> sorts_conditions_;
		std::map<std::string, std::string> iquals_conditions_;
		std::map<std::string, std::string> not_iquals_conditions_;
		std::map<std::string, std::string> greather_than_;
		std::map<std::string, std::string> smaller_than_;
		std::map<std::string, std::string> between_;
		std::map<std::string, std::vector<std::string>> in_;
		std::map<std::string, std::vector<std::string>> not_in_;
		std::vector<std::string> values_;
};


class CPW::QueryActions : public ManageJSON
{
	public:
		QueryActions();
		~QueryActions();

		std::string get_final_query() const {return final_query_;}
		Filters& get_current_filters_()
		{
			auto& var = current_filters_;
			return var;
		}
		Data::Session get_session() const {return session_;}
		Data::Statement get_query() const {return query_;}
		std::map<std::string, std::string>* get_table_rows() const {return table_rows_;}
		Poco::JSON::Array* get_result_json() const {return result_json_;}

		void ResetQuery_();
		void ComposeQuery_(TypeAction action_type, std::string table, std::string body);
		void IdentifyFilters_(Dynamic::Var dynamic_manager);
		void ExecuteQuery_();

	protected:
		std::string IqualsConditionsToString_();
		std::string ComposeInsertSentence_(std::string table, std::string body);
		std::string ComposeSelectSentence_(std::string table);
		std::string ComposeUpdateSentence_(std::string table, std::string body);
		std::string ComposeDeleteSentence_(std::string table, std::string body);

	private:
		void FillTypeActionsText_();

		std::string final_query_;
		Filters current_filters_;
		Data::Session session_;
		Data::Statement query_;
		std::map<std::string, std::string>* table_rows_;
		Poco::JSON::Array* result_json_;
		std::map<std::string, TypeQuery> type_actions_map_;
};


#endif // CPW_QUERYACTIONS_H
