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

QueryActions::QueryActions() :
	session_((Data::MySQL::Connector::registerConnector(), Data::Session("MySQL", "host=127.0.0.1;port=3306;db=cpw_woodpecker;user=root;password=mariadb_password;")))
	,query_(session_)
	,app_(Application::instance())
{
	result_json_ = new JSON::Array;
	table_rows_ = new std::map<std::string, std::string>;
	FillTypeActionsText_();
}

QueryActions::~QueryActions()
{
	delete table_rows_;
}

void QueryActions::ResetQuery_()
{
	query_.reset(session_);
}

void QueryActions::IdentifyFilters_()
{
	try
	{
		TypeQuery type;
		Dynamic::Var& data_array = get_dynamic_json_body()["pair-information"][1]["data"];

		for (std::size_t a = 0; a < data_array.size(); a++)
		{
			// Get the temporal objects
				if(data_array[a].isEmpty())
					throw std::runtime_error("Data array haves a empty object.");

				Dynamic::Var& it = data_array[a];
				if(it["type"].isEmpty())
					throw std::runtime_error("An array object don't haves a type.");

			// Search if exists
				if(ExistsType_(it["type"].toString()))
					type = type_actions_map_.find(it["type"].toString())->second;
				else
					continue;

			// Manage the type
				switch(type)
				{
					case TypeQuery::kFields:
					{
						if(it["contents"].isEmpty())
							throw std::runtime_error("contents in kFields is empty on data array index " + std::to_string(a));

						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							get_current_filters_().get_fields().push_back(it["contents"][b]);
						}
						break;
					}
					case TypeQuery::kPage:
					{
						if(it["content"].isEmpty())
							throw std::runtime_error("content in kPage is empty on data array index " + std::to_string(a));

						get_current_filters_().set_page(it["content"].toString());
						break;
					}
					case TypeQuery::kLimit:
					{
						if(it["content"].isEmpty())
							throw std::runtime_error("content in kLimit is empty on data array index " + std::to_string(a));

						get_current_filters_().set_limit(it["content"].toString());
						break;
					}
					case TypeQuery::kSort:
					{
						if(it["contents"].isEmpty())
							throw std::runtime_error("contents in kSort is empty on data array index " + std::to_string(a));

						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							get_current_filters_().get_sorts_conditions().push_back(it["contents"][b]);
						}
						break;
					}
					case TypeQuery::kIqual:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kIqual is empty on data array index " + std::to_string(a));

						get_current_filters_().get_iquals_conditions().emplace(std::make_pair
						(
							it["col"].toString()
							,it["content"].toString()
						));
						break;
					}
					case TypeQuery::kNotIqual:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kNotIqual is empty on data array index " + std::to_string(a));

						get_current_filters_().get_not_iquals_conditions().emplace(std::make_pair
						(
							it["col"].toString()
							,it["content"].toString()
						));
						break;
					}
					case TypeQuery::kGreatherThan:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kGreatherThan is empty on data array index " + std::to_string(a));

						get_current_filters_().get_greather_than().emplace(std::make_pair
						(
							it["col"].toString()
							,it["content"].toString()
						));
						break;
					}
					case TypeQuery::kSmallerThan:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kSmallerThan is empty on data array index " + std::to_string(a));

						get_current_filters_().get_smaller_than().emplace(std::make_pair
						(
							it["col"].toString()
							,it["content"].toString()
						));
						break;
					}
					case TypeQuery::kBetween:
					{
						if(it["col"].isEmpty() || it["content1"].isEmpty() || it["content2"].isEmpty())
							throw std::runtime_error("col, content1 or content2 in kBetween is empty on data array index " + std::to_string(a));

						get_current_filters_().get_between().emplace(std::make_pair
						(
							it["col"].toString()
							,std::make_pair(it["content1"].toString(), it["content2"].toString())
						));
						break;
					}
					case TypeQuery::kIn:
					{
						if(it["col"].isEmpty() || it["contents"].isEmpty())
							throw std::runtime_error("col or contents in kIn is empty on data array index " + std::to_string(a));

						std::vector<std::string> tmp_in;
						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							tmp_in.push_back(it["contents"][b]);
						}
						get_current_filters_().get_in().emplace(std::make_pair(it["col"], tmp_in));
						break;
					}
					case TypeQuery::kNotIn:
					{
						if(it["col"].isEmpty() || it["contents"].isEmpty())
							throw std::runtime_error("col or contents in kNotIn is empty on data array index " + std::to_string(a));

						std::vector<std::string> tmp_not_in;
						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							tmp_not_in.push_back(it["contents"][b]);
						}
						get_current_filters_().get_not_in().emplace(std::make_pair(it["col"], tmp_not_in));
						break;
					}
					case TypeQuery::kValues:
					{
						if(it["contents"].isEmpty())
							throw std::runtime_error("contents in kValues is empty on data array index " + std::to_string(a));

						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							get_current_filters_().get_values().push_back(std::vector<std::string> {});

							for(auto it_v: it["contents"][b])
							{
								get_current_filters_().get_values()[b].push_back(it_v.toString());
							}
						}
						break;
					}
					case TypeQuery::kSet:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kSet is empty on data array index " + std::to_string(a));

						get_current_filters_().get_set().emplace(std::make_pair
						(
							it["col"].toString()
							,it["content"].toString()
						));
						break;
					}
				}
		}
	}
	catch(std::runtime_error& error)
	{
		app_.logger().error("- Error on query_actions.cc on IdentifyFilters_(): " + std::string(error.what()));
	}
	catch(JSON::JSONException& error)
	{
		app_.logger().error("- Error on query_actions.cc on IdentifyFilters_(): " + std::string(error.what()));
	}
}

void QueryActions::ComposeQuery_(TypeAction action_type, std::string table)
{
	std::string tmp_query;

	switch(action_type)
	{
		case TypeAction::kInsert:
		{
			tmp_query = ComposeInsertSentence_(table);
			break;
		}
		case TypeAction::kSelect:
		{
			tmp_query = ComposeSelectSentence_(table);
			break;
		}
		case TypeAction::kUpdate:
		{
			tmp_query = ComposeUpdateSentence_(table);
			break;
		}
		case TypeAction::kDelete:
		{
			tmp_query = ComposeDeleteSentence_(table);
			break;
		}
	}

	final_query_ = tmp_query;
	app_.logger().information("- Final query: " + tmp_query);
}

void QueryActions::ExecuteQuery_()
{
	try
	{
		ResetQuery_();
		query_ << final_query_, now;
		CreateJSONResult_();
	}
	catch(MySQL::MySQLException& error)
	{
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.message()));
	}
	catch(JSON::JSONException& error)
	{
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.displayText()));
	}
	catch(std::exception& error)
	{
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
	}
	catch(std::runtime_error& error)
	{
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
	}
}

void QueryActions::CreateJSONResult_()
{
	// Variables
		Poco::Dynamic::Var var;
		Data::RecordSet results(query_);

	// Make JSON string
		int array_index = 0;
		for (Data::RecordSet::Iterator it = results.begin(); it != results.end(); ++it)
		{
			JSON::Object::Ptr tmp_object = new JSON::Object();

			for(size_t a = 0; a < it->fieldCount(); a++)
			{
				it->formatValues();
				var = it->get(a);
				if(!var.isEmpty())
				{
					tmp_object->set(results.columnName(a), var.toString());
				}
			}
			result_json_->set(array_index, tmp_object);
			array_index++;
		}

}

std::string QueryActions::ComposeInsertSentence_(std::string table)
{
	// Sentence type and Table
		std::vector<std::string> tmp_query = {"INSERT INTO " + table + " ("};

	// Fields
		IncorporeFields_(tmp_query);
		tmp_query.push_back(")");

	// Values
		tmp_query.push_back("VALUES");
		IncorporeValues_(tmp_query);

	tmp_query.push_back(";");

	std::string final_query = "";
	for(auto it : tmp_query)
		final_query += it + " ";

	return final_query;
}

std::string QueryActions::ComposeSelectSentence_(std::string table)
{
	// Sentence type and fields
		std::vector<std::string> tmp_query = {"SELECT"};
		IncorporeFields_(tmp_query);

	// Table
		tmp_query.push_back("FROM " + table);

	// Conditions
		IncorporeIqual_(tmp_query);
		IncorporeNotIqual_(tmp_query);
		IncorporeGreatherThan_(tmp_query);
		IncorporeSmallerThan_(tmp_query);
		IncorporeBetween_(tmp_query);
		IncorporeIn_(tmp_query);
		IncorporeNotIn_(tmp_query);

	// Sort conditions
		IncorporeSort_(tmp_query);

	// Page and Limit condition
		IncorporePageLimit_(tmp_query);

	return MakeFinalQuery_(tmp_query);
}

std::string QueryActions::ComposeUpdateSentence_(std::string table)
{
	// Sentence type and table
		std::vector<std::string> tmp_query = {"UPDATE"};
		tmp_query.push_back(table);

	// Set
		tmp_query.push_back("SET");
		IncorporeSet_(tmp_query);

	// Conditions
		IncorporeIqual_(tmp_query);
		IncorporeNotIqual_(tmp_query);
		IncorporeGreatherThan_(tmp_query);
		IncorporeSmallerThan_(tmp_query);
		IncorporeBetween_(tmp_query);
		IncorporeIn_(tmp_query);
		IncorporeNotIn_(tmp_query);

	// Sort conditions
		IncorporeSort_(tmp_query);

	// Page and Limit condition
		IncorporePageLimit_(tmp_query);

	return MakeFinalQuery_(tmp_query);
}

std::string QueryActions::ComposeDeleteSentence_(std::string table)
{
	// Sentence type and Table
		std::vector<std::string> tmp_query = {"DELETE FROM " + table};

	// Conditions
		IncorporeIqual_(tmp_query);
		IncorporeNotIqual_(tmp_query);
		IncorporeGreatherThan_(tmp_query);
		IncorporeSmallerThan_(tmp_query);
		IncorporeBetween_(tmp_query);
		IncorporeIn_(tmp_query);
		IncorporeNotIn_(tmp_query);

	// Sort conditions
		IncorporeSort_(tmp_query);

	// Page and Limit condition
		IncorporePageLimit_(tmp_query);

	return MakeFinalQuery_(tmp_query);
}

std::string QueryActions::MakeFinalQuery_(std::vector<std::string>& tmp_query)
{
	tmp_query.push_back(";");
	std::string final_query = "";

	for(auto it : tmp_query)
		final_query += it + " ";

	return final_query;
}

void QueryActions::IncorporeWhere_(std::vector<std::string>& tmp_query)
{
	auto found = std::find(tmp_query.begin(), tmp_query.end(), "WHERE");

	if(found == tmp_query.end())
		tmp_query.push_back("WHERE");
}

void QueryActions::IncorporeAND_(std::vector<std::string>& tmp_query)
{
	auto found = std::find(tmp_query.begin(), tmp_query.end(), "WHERE");

	if(*found != tmp_query.back())
		tmp_query.push_back("AND");
}

void QueryActions::IncorporeFields_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_fields().size() == 0)
		tmp_query.push_back("*");
	else
	{
		for(auto it : current_filters_.get_fields())
		{
			if(it != current_filters_.get_fields().front())
				tmp_query.push_back(",");

			tmp_query.push_back(it);
		}
	}
}

void QueryActions::IncorporePageLimit_(std::vector<std::string>& tmp_query)
{
	if(std::stoi(current_filters_.get_limit()) > 0)
	{
		int offset = std::stoi(current_filters_.get_limit()) * std::stoi(current_filters_.get_page());
		tmp_query.push_back("LIMIT");
		tmp_query.push_back(std::to_string(offset));
		tmp_query.push_back(", " + current_filters_.get_limit());
	}
	else
		tmp_query.push_back("LIMIT 0, 20 ");
}

void QueryActions::IncorporeSort_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_sorts_conditions().size() > 0)
	{
		tmp_query.push_back("ORDER BY");
		for(auto it : current_filters_.get_sorts_conditions())
		{
			if(it != current_filters_.get_sorts_conditions().front())
				tmp_query.push_back(",");

			tmp_query.push_back(it);
		}
	}
}

void QueryActions::IncorporeIqual_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_iquals_conditions().size() > 0)
	{
		IncorporeWhere_(tmp_query);

		for(auto it : current_filters_.get_iquals_conditions())
		{
			IncorporeAND_(tmp_query);

			tmp_query.push_back(it.first);
			tmp_query.push_back("=");
			tmp_query.push_back("'" + it.second + "'");
		}
	}
}

void QueryActions::IncorporeNotIqual_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_not_iquals_conditions().size() > 0)
	{
		IncorporeWhere_(tmp_query);

		for(auto it : current_filters_.get_not_iquals_conditions())
		{
			IncorporeAND_(tmp_query);

			tmp_query.push_back(it.first);
			tmp_query.push_back("<>");
			tmp_query.push_back("'" + it.second + "'");
		}
	}
}

void QueryActions::IncorporeGreatherThan_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_greather_than().size() > 0)
	{
		IncorporeWhere_(tmp_query);

		for(auto it : current_filters_.get_greather_than())
		{
			IncorporeAND_(tmp_query);

			tmp_query.push_back(it.first);
			tmp_query.push_back(">");
			tmp_query.push_back("'" + it.second + "'");
		}
	}
}

void QueryActions::IncorporeSmallerThan_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_smaller_than().size() > 0)
	{
		IncorporeWhere_(tmp_query);

		for(auto it : current_filters_.get_smaller_than())
		{
			IncorporeAND_(tmp_query);

			tmp_query.push_back(it.first);
			tmp_query.push_back("<");
			tmp_query.push_back("'" + it.second + "'");
		}
	}
}

void QueryActions::IncorporeBetween_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_between().size() > 0)
	{
		IncorporeWhere_(tmp_query);

		for(auto it : current_filters_.get_between())
		{
			IncorporeAND_(tmp_query);

			tmp_query.push_back(it.first);
			tmp_query.push_back("BETWEEN");
			tmp_query.push_back("'" + it.second.first + "'");
			tmp_query.push_back("AND");
			tmp_query.push_back("'" + it.second.second + "'");
		}
	}
}

void QueryActions::IncorporeIn_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_in().size() > 0)
	{
		IncorporeWhere_(tmp_query);

		for(auto it : current_filters_.get_in())
		{
			IncorporeAND_(tmp_query);

			tmp_query.push_back(it.first);
			tmp_query.push_back("IN (");
			for(auto it_v : it.second)
			{
				if(it_v == it.second.front())
					tmp_query.push_back("'" + it_v + "'");
				else
					tmp_query.push_back(", '" + it_v + "'");
			}
			tmp_query.push_back(")");
		}
	}
}

void QueryActions::IncorporeNotIn_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_not_in().size() > 0)
	{
		IncorporeWhere_(tmp_query);

		for(auto it : current_filters_.get_not_in())
		{
			IncorporeAND_(tmp_query);

			tmp_query.push_back(it.first);
			tmp_query.push_back("NOT IN (");
			for(auto it_v : it.second)
			{
				if(it_v == it.second.front())
					tmp_query.push_back("'" + it_v + "'");
				else
					tmp_query.push_back(", '" + it_v + "'");
			}
			tmp_query.push_back(")");
		}
	}
}

void QueryActions::IncorporeValues_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_values().size() > 0)
	{
		for(auto it : current_filters_.get_values())
		{
			if(it != current_filters_.get_values().front())
				tmp_query.push_back(",");

			tmp_query.push_back("(");
			for(auto it_sub : it)
			{
				if(it_sub != it.front())
					tmp_query.push_back(",");

				tmp_query.push_back("'" + it_sub + "'");
			}
			tmp_query.push_back(")");
		}
	}
}

void QueryActions::IncorporeSet_(std::vector<std::string>& tmp_query)
{
	if(current_filters_.get_set().size() > 0)
	{
		for(auto it : current_filters_.get_set())
		{
			if(it != *current_filters_.get_set().begin())
				tmp_query.push_back(",");

			tmp_query.push_back(it.first);
			tmp_query.push_back("=");
			tmp_query.push_back("'" + it.second + "'");
		}
	}
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
	type_actions_map_.emplace(std::make_pair("set", TypeQuery::kSet));
}

bool QueryActions::ExistsType_(std::string type)
{
	auto found = type_actions_map_.find(type);
	if(found != type_actions_map_.end())
		return true;
	else
		return false;
}
