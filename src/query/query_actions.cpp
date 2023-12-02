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
    ,app_(Application::instance())
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

void QueryActions::ExecuteQuery_(Functions::Action& action)
{
    try
    {
        affected_rows_ = query_->execute();
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.message()));
        return;
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
        return;
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what()));
        return;
    }
}

CPW::Query::Results QueryActions::MakeResults_(Functions::Action& action)
{
    try
    {
        // Variables
            Query::Results results;
            Data::RecordSet results_dataquery(*query_);

        // Default values
            if(query_.get() == nullptr)
            {
                return results;
            }

        // Save columns names
            /*for (std::size_t col = 0; col < results_dataquery.columnCount(); ++col)
                results.get_columns().push_back(results_dataquery.columnName(col));*/

        // Make Results
            for(auto& it : results_dataquery)
            {
                Query::Row row_fields;

                std::size_t col = 0;
                for(size_t a = 0; a < it.fieldCount(); a++)
                {
                    auto column_name = results_dataquery.columnName(col);
                    auto value = it.get(a);
                    row_fields.get_fields().push_back(Query::Field{column_name, Tools::RowValueFormatter(value)});
                    col++;
                }

                results.get_rows().push_back(std::move(row_fields));
            }

        // Return results
            return results;
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.message()));
        return Query::Results{};
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.what()));
        return Query::Results{};
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.what()));
        return Query::Results{};
    }

    return Query::Results();
}

JSON::Object::Ptr QueryActions::CreateJSONResult_()
{
    try
    {
        // Variables
            JSON::Object::Ptr result_json;
            Data::RecordSet results(*query_);
            JSON::Array::Ptr results_array = new JSON::Array();
            JSON::Array::Ptr columns_array = new JSON::Array();

        // Default values
            if(query_.get() == nullptr)
            {
                result_json->set("columns", columns_array);
                result_json->set("results", results_array);
                return result_json;
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

                    auto row_value_formatter = Tools::RowValueFormatter(var);
                    switch(row_value_formatter.get_row_value_type())
                    {
                        case Tools::RowValueType::kEmpty:
                            row_fields->set(row_fields->size(), "");
                            break;
                        case Tools::RowValueType::kString:
                            row_fields->set(row_fields->size(), row_value_formatter.get_value_string());
                            break;
                        case Tools::RowValueType::kInteger:
                            row_fields->set(row_fields->size(), row_value_formatter.get_value_int());
                            break;
                        case Tools::RowValueType::kFloat:
                            row_fields->set(row_fields->size(), row_value_formatter.get_value_float());
                            break;
                        default:
                            row_fields->set(row_fields->size(), "");
                            break;
                    }
                }

                results_array->set(results_array->size(), row_fields);
            }

            result_json->set("columns", columns_array);
            result_json->set("results", results_array);

            return result_json;
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.message()));
        return JSON::Object::Ptr{};
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.what()));
        return JSON::Object::Ptr{};
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.what()));
        return JSON::Object::Ptr{};
    }
}
