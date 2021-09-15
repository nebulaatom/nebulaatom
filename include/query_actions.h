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
	class QueryActions;
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Data::Keywords;

class CPW::QueryActions
{
	public:
		enum TypeAction
		{
			kInsert,
			kSelect,
			kUpdate,
			kDelete
		};

		struct Filters
		{
			std::string fields, page, limit;
			std::map<std::string, std::string> sorts_conditions;
			std::map<std::string, std::string> iquals_conditions;
		};

		QueryActions();
		~QueryActions();

		std::string get_final_query() const {return final_query_;}
		Filters get_current_filters_() const {return current_filters_;}
		Data::Session get_session() const {return session_;}
		Data::Statement get_query() const {return query_;}
		std::map<std::string, std::string>* get_table_rows() const {return table_rows_;}
		Poco::JSON::Array* get_result_json() const {return result_json_;}

		void ResetQuery_();
	protected:
		void IdentidyFilter_(const HTTPServerRequest& request);
		void CreateRows_();
		void ComposeQuery_(const HTTPServerRequest& request);

	private:
		std::string final_query_;
		Filters current_filters_;
		Data::Session session_;
		Data::Statement query_;
		std::map<std::string, std::string>* table_rows_;
		Poco::JSON::Array* result_json_;
};


#endif // CPW_QUERYACTIONS_H
