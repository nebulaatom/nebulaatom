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

using namespace CPW::Core;

QueryActions::QueryActions() :
	current_filters_(new Tools::Filters)
	,app_(Application::instance())
{
	incorporate_ = std::make_unique<Extras::IncorporateFilters>(current_filters_);
	result_json_ = new JSON::Object;
	FillTypeActionsText_();
}

QueryActions::~QueryActions()
{

}

void QueryActions::StartDatabase_()
{
	Data::MySQL::Connector::registerConnector();
	session_ = std::make_shared<Data::Session>("MySQL", "host=127.0.0.1;port=3306;db=cpw_woodpecker;user=root;password=mariadb_password;");
	query_ = std::make_shared<Data::Statement>(*session_);
}

void QueryActions::StopDatabase_()
{
	session_->close();
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
							current_filters_->get_fields().push_back({it["contents"][b], false});
						}
						break;
					}
					case TypeQuery::kPage:
					{
						if(it["content"].isEmpty())
							throw std::runtime_error("content in kPage is empty on data array index " + std::to_string(a));

						current_filters_->set_page(it["content"].toString());
						break;
					}
					case TypeQuery::kLimit:
					{
						if(it["content"].isEmpty())
							throw std::runtime_error("content in kLimit is empty on data array index " + std::to_string(a));

						current_filters_->set_limit(it["content"].toString());
						break;
					}
					case TypeQuery::kSort:
					{
						if(it["contents"].isEmpty())
							throw std::runtime_error("contents in kSort is empty on data array index " + std::to_string(a));

						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							current_filters_->get_sorts_conditions().push_back({it["contents"][b], false});
						}
						break;
					}
					case TypeQuery::kIqual:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kIqual is empty on data array index " + std::to_string(a));

						current_filters_->get_iquals_conditions().emplace(std::make_pair
						(
							it["col"].toString()
							,Extras::ValuesProperties{it["content"].toString(), true}
						));
						break;
					}
					case TypeQuery::kNotIqual:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kNotIqual is empty on data array index " + std::to_string(a));

						current_filters_->get_not_iquals_conditions().emplace(std::make_pair
						(
							it["col"].toString()
							,Extras::ValuesProperties{it["content"].toString(), true}
						));
						break;
					}
					case TypeQuery::kGreatherThan:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kGreatherThan is empty on data array index " + std::to_string(a));

						current_filters_->get_greather_than().emplace(std::make_pair
						(
							it["col"].toString()
							,Extras::ValuesProperties{it["content"].toString(), true}
						));
						break;
					}
					case TypeQuery::kSmallerThan:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kSmallerThan is empty on data array index " + std::to_string(a));

						current_filters_->get_smaller_than().emplace(std::make_pair
						(
							it["col"].toString()
							,Extras::ValuesProperties{it["content"].toString(), true}
						));
						break;
					}
					case TypeQuery::kBetween:
					{
						if(it["col"].isEmpty() || it["content1"].isEmpty() || it["content2"].isEmpty())
							throw std::runtime_error("col, content1 or content2 in kBetween is empty on data array index " + std::to_string(a));

						current_filters_->get_between().emplace(std::make_pair
						(
							it["col"].toString()
							,std::make_pair
							(
								Extras::ValuesProperties{it["content1"].toString(), true}
								,Extras::ValuesProperties{it["content2"].toString(), true}
							)
						));
						break;
					}
					case TypeQuery::kIn:
					{
						if(it["col"].isEmpty() || it["contents"].isEmpty())
							throw std::runtime_error("col or contents in kIn is empty on data array index " + std::to_string(a));

						std::vector<Extras::ValuesProperties> tmp_in;
						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							tmp_in.push_back({it["contents"][b], true});
						}
						current_filters_->get_in().emplace(std::make_pair
						(
							it["col"]
							,tmp_in
						));
						break;
					}
					case TypeQuery::kNotIn:
					{
						if(it["col"].isEmpty() || it["contents"].isEmpty())
							throw std::runtime_error("col or contents in kNotIn is empty on data array index " + std::to_string(a));

						std::vector<Extras::ValuesProperties> tmp_not_in;
						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							tmp_not_in.push_back({it["contents"][b], true});
						}
						current_filters_->get_not_in().emplace(std::make_pair(it["col"], tmp_not_in));
						break;
					}
					case TypeQuery::kValues:
					{
						if(it["contents"].isEmpty())
							throw std::runtime_error("contents in kValues is empty on data array index " + std::to_string(a));

						for(std::size_t b = 0; b < it["contents"].size(); b++)
						{
							current_filters_->get_values().push_back(std::vector<Extras::ValuesProperties> {});

							for(auto it_v: it["contents"][b])
							{
								current_filters_->get_values()[b].push_back({it_v.toString(), true});
							}
						}
						break;
					}
					case TypeQuery::kSet:
					{
						if(it["content"].isEmpty() || it["col"].isEmpty())
							throw std::runtime_error("content or col in kSet is empty on data array index " + std::to_string(a));

						current_filters_->get_set().emplace(std::make_pair
						(
							it["col"].toString()
							,Extras::ValuesProperties{it["content"].toString(), true}
						));
						break;
					}
					case TypeQuery::kJoins:
					{
						if(it["join-type"].isEmpty() || it["table"].isEmpty() || it["on"].isEmpty())
							throw std::runtime_error("join-type, table or on in kJoins is empty on data array index " + std::to_string(a));

						std::map<std::string, Extras::ValuesProperties> tmp_joins;

						for(std::size_t b = 0; b < it["on"].size(); b++)
						{
							tmp_joins.emplace(std::make_pair
							(
								it["on"][b]["col"].toString()
								,Extras::ValuesProperties{it["on"][b]["value"].toString(), false}
							));
						}
						current_filters_->get_joins().emplace
						(
							std::make_pair
							(
								std::array<std::string, 2>{it["join-type"].toString(), it["table"].toString()}
								,tmp_joins
							)
						);
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

bool QueryActions::ExecuteQuery_(HTTPServerResponse& response)
{
	try
	{
		StartDatabase_();
		*query_ << final_query_, now;
		StopDatabase_();

		CreateJSONResult_();
	}
	catch(MySQL::MySQLException& error)
	{
		GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, std::string(error.message()));
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.message()));
		return false;
	}
	catch(JSON::JSONException& error)
	{
		GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, std::string(error.message()));
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.displayText()));
		return false;
	}
	catch(std::exception& error)
	{
		GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, std::string(error.what()));
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
		return false;
	}
	catch(std::runtime_error& error)
	{
		GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, std::string(error.what()));
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
		return false;
	}

	return true;
}

bool QueryActions::ExecuteQuery_()
{
	try
	{
		StartDatabase_();
		*query_ << final_query_, now;
		StopDatabase_();

		CreateJSONResult_();
	}
	catch(MySQL::MySQLException& error)
	{
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.message()));
		return false;
	}
	catch(JSON::JSONException& error)
	{
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.displayText()));
		return false;
	}
	catch(std::exception& error)
	{
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
		return false;
	}
	catch(std::runtime_error& error)
	{
		app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
		return false;
	}

	return true;
}

void QueryActions::CreateJSONResult_()
{
	// Variables
		Data::RecordSet results(*query_);
		JSON::Array::Ptr tmp_array = new JSON::Array();

	// Make JSON string
		for(auto& it : results)
		{
			JSON::Object::Ptr tmp_object = new JSON::Object();

			for(size_t a = 0; a < it.fieldCount(); a++)
			{
				auto var = it.get(a);

				if(var.isEmpty())
                    tmp_object->set(results.columnName(a), "");
                else if(var.isNumeric())
                    if(var.isInteger())
                        tmp_object->set(results.columnName(a), std::stoi(var.toString()));
                	else
                        tmp_object->set(results.columnName(a), std::stof(var.toString()));
                else if(var.isDate() || var.isDateTime())
                {
                    DateTime date;
                    int diff;

                    if(DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, var.toString(), date, diff))
                    {
                        DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, var.toString(), date, diff);
                        auto date_string = DateTimeFormatter::format(date, DateTimeFormat::SORTABLE_FORMAT);

                        tmp_object->set(results.columnName(a), date_string);
                    }
                    else
                        tmp_object->set(results.columnName(a), var.toString());
                }
                else
                    tmp_object->set(results.columnName(a), var.toString());
			}
			tmp_array->set(tmp_array->size(), tmp_object);
		}
		result_json_->set("results", tmp_array);

}

std::string QueryActions::ComposeInsertSentence_(std::string table)
{
	// Sentence type and Table
		std::vector<std::string> tmp_query = {"INSERT INTO " + table + " ("};

	// Fields
		incorporate_->IncorporateFields_(tmp_query);
		tmp_query.push_back(")");

	// Values
		tmp_query.push_back("VALUES");
		incorporate_->IncorporateValues_(tmp_query);

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
		incorporate_->IncorporateFields_(tmp_query);

	// Table
		tmp_query.push_back("FROM " + table);

	// Joins
		incorporate_->IncorporateJoins_(tmp_query);

	// Conditions
		incorporate_->IncorporateIqual_(tmp_query);
		incorporate_->IncorporateNotIqual_(tmp_query);
		incorporate_->IncorporateGreatherThan_(tmp_query);
		incorporate_->IncorporateSmallerThan_(tmp_query);
		incorporate_->IncorporateBetween_(tmp_query);
		incorporate_->IncorporateIn_(tmp_query);
		incorporate_->IncorporateNotIn_(tmp_query);

	// Sort conditions
		incorporate_->IncorporateSort_(tmp_query);

	// Page and Limit condition
		incorporate_->IncorporatePageLimit_(tmp_query, true);

	return MakeFinalQuery_(tmp_query);
}

std::string QueryActions::ComposeUpdateSentence_(std::string table)
{
	// Sentence type and table
		std::vector<std::string> tmp_query = {"UPDATE"};
		tmp_query.push_back(table);

	// Joins
		incorporate_->IncorporateJoins_(tmp_query);

	// Set
		tmp_query.push_back("SET");
		incorporate_->IncorporateSet_(tmp_query);

	// Conditions
		incorporate_->IncorporateIqual_(tmp_query);
		incorporate_->IncorporateNotIqual_(tmp_query);
		incorporate_->IncorporateGreatherThan_(tmp_query);
		incorporate_->IncorporateSmallerThan_(tmp_query);
		incorporate_->IncorporateBetween_(tmp_query);
		incorporate_->IncorporateIn_(tmp_query);
		incorporate_->IncorporateNotIn_(tmp_query);

	// Sort conditions
		incorporate_->IncorporateSort_(tmp_query);

	// Page and Limit condition
		incorporate_->IncorporatePageLimit_(tmp_query, false);

	return MakeFinalQuery_(tmp_query);
}

std::string QueryActions::ComposeDeleteSentence_(std::string table)
{
	// Sentence type and Table
		std::vector<std::string> tmp_query = {"DELETE FROM " + table};

	// Joins
		incorporate_->IncorporateJoins_(tmp_query);

	// Conditions
		incorporate_->IncorporateIqual_(tmp_query);
		incorporate_->IncorporateNotIqual_(tmp_query);
		incorporate_->IncorporateGreatherThan_(tmp_query);
		incorporate_->IncorporateSmallerThan_(tmp_query);
		incorporate_->IncorporateBetween_(tmp_query);
		incorporate_->IncorporateIn_(tmp_query);
		incorporate_->IncorporateNotIn_(tmp_query);

	// Sort conditions
		incorporate_->IncorporateSort_(tmp_query);

	// Page and Limit condition
		incorporate_->IncorporatePageLimit_(tmp_query, false);

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
