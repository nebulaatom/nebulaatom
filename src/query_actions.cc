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

ManageJSON::ManageJSON()
{

}

ManageJSON::~ManageJSON()
{

}

std::string ManageJSON::ReadBody_(std::istream& stream)
{
	std::string json_body;
	StreamCopier::copyToString(stream, json_body);

	if(json_body.empty())
		return "";

	return json_body;
}

void ManageJSON::Parse_(std::string string_to_parse)
{
	JSON::Parser parser;
	JSON::Object::Ptr object_json = parser.parse(string_to_parse).extract<JSON::Object::Ptr>();
	dynamic_json_body_ = *object_json;
	if(!dynamic_json_body_["pair-information"].isEmpty())
		dynamic_manager_ = object_json->get("pair-information");
}

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
	result_json_ = new Poco::JSON::Array;
	table_rows_ = new std::map<std::string, std::string>;
	FillTypeActionsText_();
}

QueryActions::~QueryActions()
{
	delete result_json_;
	delete table_rows_;
}

void QueryActions::ResetQuery_()
{
	query_.reset(session_);
}

void QueryActions::IdentifyFilters_(Dynamic::Var dynamic_manager)
{
	try
	{
		get_dynamic_manager() = dynamic_manager;
		TypeQuery type;

		// Get the main array
			JSON::Array::Ptr main_array = get_dynamic_manager().extract<JSON::Array::Ptr>();

		// Create the data json array and object
			JSON::Object::Ptr data_object = main_array->getObject(1);
			JSON::Array::Ptr data_array = data_object->getArray("data");

		Dynamic::Var it;
		for (std::size_t a = 0; a < data_array->size(); a++)
		{
			JSON::Object::Ptr tmp_object = data_array->getObject(a);

			it = tmp_object->get("type");

			// Search if exists
				auto found = type_actions_map_.find(it.toString());
				if(found != type_actions_map_.end())
					type = found->second;
				else
					continue;

			std::cout << "\nFound type: " << it.toString();

			switch(type)
			{
				case TypeQuery::kFields:
				{
					if(!tmp_object->isArray("contents"))
						throw std::runtime_error("contents is not array on tmp_object\n");

					JSON::Array::Ptr tmp_object_contents = tmp_object->getArray("contents");
					for(std::size_t b = 0; b < tmp_object_contents->size(); b++)
					{
						auto value = tmp_object_contents->get(b);
						get_current_filters_().get_fields().push_back(value.toString());
					}
					break;
				}
				case TypeQuery::kPage:
				{
					auto value = tmp_object->get("content");
					get_current_filters_().set_page(value.toString());
					break;
				}
				case TypeQuery::kLimit:
				{
					auto value = tmp_object->get("content");
					get_current_filters_().set_limit(value.toString());
					break;
				}
				case TypeQuery::kSort:
				{
					break;
				}
				case TypeQuery::kIqual:
				{
					break;
				}
				case TypeQuery::kNotIqual:
				{
					break;
				}
				case TypeQuery::kGreatherThan:
				{
					break;
				}
				case TypeQuery::kSmallerThan:
				{
					break;
				}
				case TypeQuery::kBetween:
				{
					break;
				}
				case TypeQuery::kIn:
				{
					break;
				}
				case TypeQuery::kNotIn:
				{
					break;
				}
				case TypeQuery::kValues:
				{
					break;
				}
			}
		}
	}
	catch(std::runtime_error& error)
	{
		std::cout << "\nError on query_actions.cc on IdentifyFilters_(): " << error.what();
	}
}

void QueryActions::ComposeQuery_(TypeAction action_type, std::string table, std::string body)
{
	std::string tmp_query;

	switch(action_type)
	{
		case TypeAction::kInsert:
		{
			tmp_query = ComposeInsertSentence_(table, body);
			break;
		}
		case TypeAction::kSelect:
		{
			tmp_query = ComposeSelectSentence_(table);
			break;
		}
		case TypeAction::kUpdate:
		{
			tmp_query = ComposeUpdateSentence_(table, body);
			break;
		}
		case TypeAction::kDelete:
		{
			tmp_query = ComposeDeleteSentence_(table, body);
			break;
		}
	}

	std::cout << "\nFinal query: " << tmp_query;
}

void CreateRows_(TypeAction action_type)
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

std::string QueryActions::ComposeInsertSentence_(std::string table, std::string body)
{

}

std::string QueryActions::ComposeSelectSentence_(std::string table)
{
	std::string tmp_query = "SELECT ";
	if(current_filters_.get_fields() == "")
		tmp_query += "* FROM ";
	else
		tmp_query += current_filters_.get_fields() + " ";

	tmp_query += "FROM " + table + " ";
	if(current_filters_.get_iquals_conditions().size() > 0)
		tmp_query += "WHERE " + IqualsConditionsToString_() + " ";

	if(current_filters_.get_sorts_conditions() != "")
		tmp_query += "ORDER BY " + current_filters_.get_sorts_conditions() + " ";

	if(std::stoi(current_filters_.get_limit()) > 0)
	{
		int offset = std::stoi(current_filters_.get_limit()) * std::stoi(current_filters_.get_page());
		tmp_query += "LIMIT " + std::to_string(offset) + ", " + current_filters_.get_limit() + " ";
	}
	else
		tmp_query += "LIMIT 0, 20 ";

	tmp_query += ";";

	return tmp_query;
}

std::string QueryActions::ComposeUpdateSentence_(std::string table, std::string body)
{

}

std::string QueryActions::ComposeDeleteSentence_(std::string table, std::string body)
{

}

void QueryActions::FillTypeActionsText_()
{
	type_actions_map_.emplace(std::make_pair("fields", TypeQuery::kFields));
	type_actions_map_.emplace(std::make_pair("page", TypeQuery::kPage));
	type_actions_map_.emplace(std::make_pair("limit", TypeQuery::kLimit));
	type_actions_map_.emplace(std::make_pair("sort", TypeQuery::kSort));
	type_actions_map_.emplace(std::make_pair("iqual", TypeQuery::kIqual));
	type_actions_map_.emplace(std::make_pair("notiqual", TypeQuery::kNotIqual));
	type_actions_map_.emplace(std::make_pair("greatherthan", TypeQuery::kGreatherThan));
	type_actions_map_.emplace(std::make_pair("smallerthan", TypeQuery::kSmallerThan));
	type_actions_map_.emplace(std::make_pair("between", TypeQuery::kBetween));
	type_actions_map_.emplace(std::make_pair("in", TypeQuery::kIn));
	type_actions_map_.emplace(std::make_pair("notin", TypeQuery::kNotIn));
	type_actions_map_.emplace(std::make_pair("values", TypeQuery::kValues));
}
