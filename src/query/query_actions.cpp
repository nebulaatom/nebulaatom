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

#include "query/query_actions.h"

using namespace CPW::Query;

QueryActions::QueryActions() :
    final_query_("")
    ,affected_rows_(0)
    ,current_filters_(new Filters::FiltersManager)
    ,result_json_(new JSON::Object)
    ,app_(Application::instance())
    ,row_value_formatter_(new Tools::RowValueFormatter)
{

}

QueryActions::~QueryActions()
{

}

void QueryActions::IdentifyFilters_()
{
    try
    {
        Filters::FilterType type;
        auto data_array = get_json_body();

        for (std::size_t a = 0; a < data_array->size(); a++)
        {
            // Get the filter object
                if(data_array->get(a).isEmpty())
                    throw std::runtime_error("Data array haves a empty object.");

            // Get the type of filter object
                auto filter = data_array->getObject(a);
                if(filter->get("type").isEmpty())
                    throw std::runtime_error("An array object don't haves a type.");

            // Search if exists
                if(current_filters_->ExistsType_(filter->get("type").toString()))
                    type = current_filters_->get_filters_type_map().find(filter->get("type").toString())->second;
                else
                    continue;

                auto filter_var = data_array->get(a);

            // Verify parameterized
                if(!VerifyFilterParameretized_(type))
                    continue;

            // Manage the type
                ManageFilterType_(type, filter_var);
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

void QueryActions::ResetFilters_()
{
    current_filters_->get_fields_filter()->get_filter_elements().clear();
    current_filters_->get_sort_filter()->get_filter_elements().clear();
    current_filters_->get_general_filter()->get_filter_elements().set_page("0");
    current_filters_->get_general_filter()->get_filter_elements().set_limit("20");
    current_filters_->get_general_filter()->get_filter_elements().set_as("");
    current_filters_->get_iquals_filter()->get_filter_elements().clear();
    current_filters_->get_range_filter()->get_filter_elements().clear();
    current_filters_->get_list_filter()->get_filter_elements().clear();
    current_filters_->get_like_filter()->get_filter_elements().clear();
    current_filters_->get_join_filter()->get_filter_elements().clear();
    current_filters_->get_group_filter()->get_filter_elements().clear();
    current_filters_->get_values_filter()->get_filter_elements().clear();
    current_filters_->get_set_filter()->get_filter_elements().clear();
}

bool QueryActions::ComposeQuery_(TypeAction action_type, std::string table)
{
    try
    {
        // Compose query string
            std::string tmp_query;

            switch(action_type)
            {
                case TypeAction::kInsert: tmp_query = ComposeInsertSentence_(table); break;
                case TypeAction::kSelect: tmp_query = ComposeSelectSentence_(table); break;
                case TypeAction::kUpdate: tmp_query = ComposeUpdateSentence_(table); break;
                case TypeAction::kDelete: tmp_query = ComposeDeleteSentence_(table); break;
            }

        // Create de query statement
            session_ = Query::DatabaseManager::StartSessionMySQL_();
            if(session_.get() == nullptr)
            {
                throw MySQL::MySQLException("Error to connect to database server");
                return false;
            }

            query_ = std::make_shared<Data::Statement>(*session_);

        // Set the query
            final_query_ = std::move(tmp_query);
            *query_ << final_query_;

        // Set the parameters
            for(auto& par : query_parameters_)
            {
                switch(par.get_row_value_type())
                {
                    case Tools::RowValueType::kEmpty:
                    {
                        auto value = Poco::Nullable<int>();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::RowValueType::kString:
                    {
                        auto& value = par.get_value_string();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::RowValueType::kInteger:
                    {
                        auto& value = par.get_value_int();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::RowValueType::kFloat:
                    {
                        auto& value = par.get_value_float();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::RowValueType::kBoolean:
                    {
                        auto& value = par.get_value_bool();
                        *query_ , use(value);
                        break;
                    }
                }
            }

        // Return
            app_.logger().information("- Final query: " + query_->toString());
            return true;
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on query_actions.cc on ComposeQuery_(): " + std::string(error.message()));
        return false;
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on query_actions.cc on ComposeQuery_(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on query_actions.cc on ComposeQuery_(): " + std::string(error.what()));
        return false;
    }
    return false;
}

bool QueryActions::ExecuteQuery_(HTTPServerResponse& response)
{
    try
    {
        affected_rows_ = query_->execute();
    }
    catch(MySQL::MySQLException& error)
    {
        GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, std::string(error.message()));
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.message()));
        return false;
    }
    catch(std::runtime_error& error)
    {
        GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, std::string(error.what()));
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
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
        affected_rows_ = query_->execute();
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.message()));
        return false;
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
        return false;
    }

    return true;
}

bool QueryActions::CreateJSONResult_()
{
    try
    {
        // Variables
            result_json_->clear();
            Data::RecordSet results(*query_);
            JSON::Array::Ptr results_array = new JSON::Array();
            JSON::Array::Ptr columns_array = new JSON::Array();

        // Default values
            if(query_.get() == nullptr)
            {
                result_json_->set("columns", columns_array);
                result_json_->set("results", results_array);
                return false;
            }

        // Save columns names
            for (std::size_t col = 0; col < results.columnCount(); ++col)
                columns_array->set(columns_array->size(), results.columnName(col));

        // Make JSON data
            for(auto& it : results)
            {
                JSON::Array::Ptr row_fields = new JSON::Array();

                for(size_t a = 0; a < it.fieldCount(); a++)
                {
                    auto var = it.get(a);

                    row_value_formatter_.reset(new Tools::RowValueFormatter(var));
                    row_value_formatter_->Format_();
                    switch(row_value_formatter_->get_row_value_type())
                    {
                        case Tools::RowValueType::kEmpty:
                            row_fields->set(row_fields->size(), "");
                            break;
                        case Tools::RowValueType::kString:
                            row_fields->set(row_fields->size(), row_value_formatter_->get_value_string());
                            break;
                        case Tools::RowValueType::kInteger:
                            row_fields->set(row_fields->size(), row_value_formatter_->get_value_int());
                            break;
                        case Tools::RowValueType::kFloat:
                            row_fields->set(row_fields->size(), row_value_formatter_->get_value_float());
                            break;
                        default:
                            row_fields->set(row_fields->size(), "");
                            break;
                    }
                }

                results_array->set(results_array->size(), row_fields);
            }

            result_json_->set("columns", columns_array);
            result_json_->set("results", results_array);
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.message()));
        return false;
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.what()));
        return false;
    }

    return true;
}

std::string QueryActions::ComposeInsertSentence_(std::string table)
{
    // Sentence type and Table
        std::vector<std::string> tmp_query = {"INSERT INTO " + table + " ("};

    // Fields
        current_filters_->get_fields_filter()->Incorporate_(tmp_query, query_parameters_);
        tmp_query.push_back(")");

    // Values
        current_filters_->get_values_filter()->Incorporate_(tmp_query, query_parameters_);

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

        if(current_filters_->get_fields_filter()->get_filter_elements().size() < 1)
            tmp_query.push_back("*");
        else
            current_filters_->get_fields_filter()->Incorporate_(tmp_query, query_parameters_);

    // Table
        tmp_query.push_back("FROM " + table);
        current_filters_->get_general_filter()->IncorporateAS_(tmp_query, query_parameters_);

    // Joins
        current_filters_->get_join_filter()->Incorporate_(tmp_query, query_parameters_);

    // Conditions
        current_filters_->get_iquals_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_range_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_list_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_like_filter()->Incorporate_(tmp_query, query_parameters_);

    // Group and Sort conditions
        current_filters_->get_group_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_sort_filter()->Incorporate_(tmp_query, query_parameters_);

    // Page and Limit condition
        current_filters_->get_general_filter()->get_filter_elements().set_pagination(true);
        current_filters_->get_general_filter()->Incorporate_(tmp_query, query_parameters_);

    return MakeFinalQuery_(tmp_query);
}

std::string QueryActions::ComposeUpdateSentence_(std::string table)
{
    // Sentence type and table
        std::vector<std::string> tmp_query = {"UPDATE"};
        tmp_query.push_back(table);
        current_filters_->get_general_filter()->IncorporateAS_(tmp_query, query_parameters_);

    // Joins
        current_filters_->get_join_filter()->Incorporate_(tmp_query, query_parameters_);

    // Set
        current_filters_->get_set_filter()->Incorporate_(tmp_query, query_parameters_);

    // Conditions
        current_filters_->get_iquals_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_range_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_list_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_like_filter()->Incorporate_(tmp_query, query_parameters_);

    // Sort conditions
        current_filters_->get_sort_filter()->Incorporate_(tmp_query, query_parameters_);

    // Page and Limit condition
        current_filters_->get_general_filter()->get_filter_elements().set_pagination(false);
        current_filters_->get_general_filter()->Incorporate_(tmp_query, query_parameters_);

    return MakeFinalQuery_(tmp_query);
}

std::string QueryActions::ComposeDeleteSentence_(std::string table)
{
    // Sentence type and Table
        std::vector<std::string> tmp_query = {"DELETE"};
        current_filters_->get_fields_filter()->Incorporate_(tmp_query, query_parameters_);

        tmp_query.push_back("FROM " + table);

    // Joins
        current_filters_->get_join_filter()->Incorporate_(tmp_query, query_parameters_);

    // Conditions
        current_filters_->get_iquals_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_range_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_list_filter()->Incorporate_(tmp_query, query_parameters_);
        current_filters_->get_like_filter()->Incorporate_(tmp_query, query_parameters_);

    // Sort conditions
        current_filters_->get_sort_filter()->Incorporate_(tmp_query, query_parameters_);

    // Page and Limit condition
        current_filters_->get_general_filter()->get_filter_elements().set_pagination(false);
        current_filters_->get_general_filter()->Incorporate_(tmp_query, query_parameters_);

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

bool QueryActions::VerifyFilterParameretized_(Filters::FilterType type)
{
    switch(type)
    {
        case Filters::FilterType::kFields:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kFields);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kSort:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kSort);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kGeneral:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kGeneral);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kIqual:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kIqual);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kRange:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kRange);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kList:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kList);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kLike:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kLike);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kJoin:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kJoin);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kGroup:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kGroup);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kValues:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kValues);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kSet:
        {
            auto found = current_filters_->get_filters_parameterized().find(Filters::FilterType::kSet);
            if(found != current_filters_->get_filters_parameterized().end())
                return found->second;
            break;
        }
        case Filters::FilterType::kUnknown:
        default:
            break;
    }
    return false;
}

void QueryActions::ManageFilterType_(Filters::FilterType type, Dynamic::Var& filter_var)
{
    switch(type)
    {
        case Filters::FilterType::kFields: current_filters_->get_fields_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kSort: current_filters_->get_sort_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kGeneral: current_filters_->get_general_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kIqual: current_filters_->get_iquals_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kRange: current_filters_->get_range_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kList: current_filters_->get_list_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kLike: current_filters_->get_like_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kJoin: current_filters_->get_join_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kGroup: current_filters_->get_group_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kValues: current_filters_->get_values_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kSet: current_filters_->get_set_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kUnknown:
        default:
            break;
    }
}
