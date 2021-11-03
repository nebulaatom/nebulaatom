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

#include "query_actions.h"

using namespace CPW;

Filters::Filters() :
	page_("0")
	,limit_("20")
{

}

Filters::~Filters()
{

}

QueryActions::QueryActions() :
	session_((Data::MySQL::Connector::registerConnector(), Data::Session("MySQL", "host=127.0.0.1;port=3306;db=cpw_woodpecker;user=root;password=mariadb_password;"))),
	query_(session_)
{
	table_rows_ = new std::map<std::string, std::string>;
	result_json_ = new Poco::JSON::Array;
}

QueryActions::~QueryActions()
{

}

void QueryActions::ResetQuery_()
{
	query_.reset(session_);
}

void QueryActions::IdentifyFilters_(HTTPServerRequest& request)
{
	URI initial_uri(request.getURI());

	auto query_parameters = initial_uri.getQueryParameters();
	for(auto it : query_parameters)
	{
		std::cout << "\n" << it.first << "=" << it.second;
		if(it.first == "_fields")
			current_filters_.set_fields(it.second);
		else if(it.first == "_page")
			current_filters_.set_page(it.second);
		else if(it.first == "_limit")
			current_filters_.set_limit(it.second);
		else if(it.first == "_sort")
			current_filters_.set_sorts_conditions(it.second);
		else
		{
			current_filters_.get_iquals_conditions().emplace(std::make_pair(it.first, it.second));
		}
	}
}

void QueryActions::ComposeQuery_(TypeAction action_type, std::string table, std::string body)
{

}

void QueryActions::ExecuteQuery_()
{

}

std::string QueryActions::IqualsConditionsToString_()
{
	std::string iquals_conditions = "";
	bool first = true;

	for(auto it : current_filters_.get_iquals_conditions())
	{
		if(first)
		{
			iquals_conditions += it.first + "='" + it.second + "' ";
			first = false;
		}
		else
			iquals_conditions += "AND " + it.first + "='" + it.second + "' ";
	}

	return iquals_conditions;
}

