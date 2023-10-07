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
    //,current_filters_(new Filters::FiltersManager)
    ,result_json_(new JSON::Object)
    ,app_(Application::instance())
    ,row_value_formatter_(new Tools::RowValueFormatter)
{

}

QueryActions::~QueryActions()
{

}

void QueryActions::IdentifyParameters_(Functions::Action& action)
{
    try
    {
        auto data_array = get_json_body();

        for (std::size_t a = 0; a < data_array->size(); a++)
        {
            // Get the action object
                if(data_array->get(a).isEmpty())
                {
                    app_.logger().error("Data array haves an empty action.");
                    continue;
                }

                auto action_object = data_array->getObject(a);

            // Get the action identifier
                if(action_object->get("action_id").isEmpty() || !action_object->get("action_id").isString())
                {
                    app_.logger().error("The action object does not have an action_id String Object.");
                    continue;
                }

                auto action_id = action_object->get("action_id").toString();

            // Verify Action identifier
                if(action_id != action.get_identifier())
                    continue;

            // Get the parameters object
                if(action_object->get("parameters").isEmpty() || !action_object->get("parameters").isArray())
                {
                    app_.logger().error("The action object does not have a parameters array.");
                    continue;
                }

                auto parameters_array = action_object->getArray("parameters");

            // Iterate over parameters array
                for(std::size_t b = 0; b < parameters_array->size(); b++)
                {
                    // Get parameter object
                    if(parameters_array->get(b).isEmpty())
                    {
                        app_.logger().error("Parameters array haves an empty element.");
                        continue;
                    }

                    auto parameter_object = parameters_array->getObject(b);

                    // Get parameter name
                    if(parameter_object->get("name").isEmpty() || !parameter_object->get("name").isString())
                    {
                        app_.logger().error("Parameter name is not a String Object.");
                        continue;
                    }

                    auto parameter_name = parameter_object->get("name").toString();

                    // Get parameter value
                    if(parameter_object->get("value").isEmpty())
                    {
                        app_.logger().error("Parameter value is empty.");
                        continue;
                    }

                    auto parameter_value = parameter_object->get("value");

                    // Save parameter
                    auto new_parameter = Query::Parameter(parameter_name, Tools::RowValueFormatter{parameter_value});
                    action.get_parameters().push_back(std::move(new_parameter));
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


bool QueryActions::ComposeQuery_(Functions::Action& action)
{
    try
    {
        // Create de query statement
            session_ = Query::DatabaseManager::StartSessionMySQL_();
            if(session_.get() == nullptr)
            {
                throw MySQL::MySQLException("Error to connect to database server.");
                return false;
            }

            query_ = std::make_shared<Data::Statement>(*session_);

        // Set the query
            *query_ << action.get_sql_code();

        // Set the parameters
            for(auto& param : action.get_parameters())
            {
                switch(param.get_value().get_row_value_type())
                {
                    case Tools::RowValueType::kEmpty:
                    {
                        auto value = Poco::Nullable<int>();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::RowValueType::kString:
                    {
                        auto& value = param.get_value().get_value_string();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::RowValueType::kInteger:
                    {
                        auto& value = param.get_value().get_value_int();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::RowValueType::kFloat:
                    {
                        auto& value = param.get_value().get_value_float();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::RowValueType::kBoolean:
                    {
                        auto& value = param.get_value().get_value_bool();
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

bool QueryActions::ExecuteQuery_(HTTPServerResponse* response)
{
    try
    {
        affected_rows_ = query_->execute();
    }
    catch(MySQL::MySQLException& error)
    {
        GenericResponse_(*response, HTTPResponse::HTTP_BAD_REQUEST, std::string(error.message()));
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.message()));
        return false;
    }
    catch(std::runtime_error& error)
    {
        GenericResponse_(*response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, std::string(error.what()));
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
    {
        GenericResponse_(*response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, std::string(error.what()));
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
        current_filters_->get_general_filter()->IncorporateSelected_(tmp_query, Filters::GeneralFilterElement::Type::kAs);

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
        current_filters_->get_general_filter()->IncorporateSelected_(tmp_query, Filters::GeneralFilterElement::Type::kPageLimit);

    return MakeFinalQuery_(tmp_query);
}

std::string QueryActions::ComposeUpdateSentence_(std::string table)
{
    // Sentence type and table
        std::vector<std::string> tmp_query = {"UPDATE"};
        tmp_query.push_back(table);
        current_filters_->get_general_filter()->IncorporateSelected_(tmp_query, Filters::GeneralFilterElement::Type::kAs);

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

    // Limit condition
        current_filters_->get_general_filter()->IncorporateSelected_(tmp_query, Filters::GeneralFilterElement::Type::kLimit);

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

    // Limit condition
        current_filters_->get_general_filter()->IncorporateSelected_(tmp_query, Filters::GeneralFilterElement::Type::kLimit);

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

void QueryActions::ManageFilterType_(Filters::FilterType type, Dynamic::Var& filter_var)
{
    switch(type)
    {
        case Filters::FilterType::kGeneral: current_filters_->get_general_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kIqual: current_filters_->get_iquals_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kRange: current_filters_->get_range_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kList: current_filters_->get_list_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kLike: current_filters_->get_like_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kValues: current_filters_->get_values_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kSet: current_filters_->get_set_filter()->Identify_(filter_var); break;
        case Filters::FilterType::kUnknown:
        case Filters::FilterType::kFields:
        case Filters::FilterType::kSort:
        case Filters::FilterType::kJoin:
        case Filters::FilterType::kGroup:
        default:
            break;
    }
}

void QueryActions::ManageFilterTypeFromFiles_(Filters::FilterType type, Dynamic::Var& filter_var)
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
