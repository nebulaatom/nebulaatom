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
#include "query/parameter.h"
#include "query/results.h"
#include "tools/row_value_formatter.h"

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
        auto& data_array = get_json_body();

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

                    // Find if exists parameter
                    auto found_param = std::find_if(action.get_parameters().begin(), action.get_parameters().end(), [parameter_name](Query::Parameter parameter)
                    {
                        return parameter.get_name() == parameter_name;
                    });

                    // Remplace parameter value
                    if(found_param != action.get_parameters().end())
                    {
                        if(found_param->get_editable())
                        {
                            auto index = std::distance(action.get_parameters().begin(), found_param);
                            action.get_parameters().erase(found_param);

                            // Insert new element
                            auto parameter = Query::Parameter(parameter_name, parameter_value, true);
                            action.get_parameters().insert(action.get_parameters().begin() + index, std::move(parameter));
                        }
                    }

                }

        }
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "- Error on query_actions.cc on IdentifyFilters_(): " + std::string(error.what());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
    }
    catch(JSON::JSONException& error)
    {
        std::string string_error = "- Error on query_actions.cc on IdentifyFilters_(): " + std::string(error.what());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
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
                // Verify conditional parameter
                if(param.get_parameter_type() == ParameterType::kConditional)
                {
                    auto row_value = param.get_result()->FindField_(param.get_conditional_field());
                    param.set_value(row_value->get_value());
                }

                // Add final value to query
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
        std::string string_error = "- Error on query_actions.cc on ComposeQuery_(): " + std::string(error.message());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
        return false;
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "- Error on query_actions.cc on ComposeQuery_(): " + std::string(error.what());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
        return false;
    }
    catch(std::exception& error)
    {
        std::string string_error = "- Error on query_actions.cc on ComposeQuery_(): " + std::string(error.what());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
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
        std::string string_error = "- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.message());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
        return;
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
        return;
    }
    catch(std::exception& error)
    {
        std::string string_error = "- Error on query_actions.cc on ExecuteQuery_(): " + std::string(error.what());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
        return;
    }
}

void QueryActions::MakeResults_(Functions::Action& action)
{
    try
    {
        // Variables
            Data::RecordSet results_dataquery(*query_);

        // Default values
            if(query_.get() == nullptr)
                return;

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

                action.get_results()->get_rows().push_back(std::move(row_fields));
            }
    }
    catch(JSON::JSONException& error)
    {
        std::string string_error = "- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.message());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
        return;
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.what());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
        return;
    }
    catch(std::exception& error)
    {
        std::string string_error = "- Error on query_actions.cc on CreateJSONResult_(): " + std::string(error.what());
        app_.logger().error(string_error);
        StablishActionError_(action, string_error);
        return;
    }

    return;
}

JSON::Object::Ptr QueryActions::CreateJSONResult_()
{
    try
    {
        // Variables
            JSON::Object::Ptr result_json = new JSON::Object();
            JSON::Array::Ptr data_array = new JSON::Array();
            Data::RecordSet results(*query_);

        // Default values
            if(query_.get() == nullptr)
            {
                result_json->set("data", data_array);
                return result_json;
            }

        // Make JSON data
            for(auto& it : results)
            {
                JSON::Object::Ptr row_fields = new JSON::Object();

                for(size_t a = 0; a < it.fieldCount(); a++)
                {
                    auto var = it.get(a);
                    auto row_value = Tools::RowValueFormatter(var);
                    switch(row_value.get_row_value_type())
                    {
                        case Tools::RowValueType::kBoolean:
                            row_fields->set(results.columnName(a), row_value.get_value_bool());
                            break;
                        case Tools::RowValueType::kFloat:
                            row_fields->set(results.columnName(a), row_value.get_value_float());
                            break;
                        case Tools::RowValueType::kInteger:
                            row_fields->set(results.columnName(a), row_value.get_value_int());
                            break;
                        case Tools::RowValueType::kString:
                            row_fields->set(results.columnName(a), row_value.get_value_string());
                            break;
                        case Tools::RowValueType::kEmpty:
                            row_fields->set(results.columnName(a), "");
                            break;
                    }
                }

                data_array->set(data_array->size(), row_fields);
            }

            result_json->set("data", data_array);

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

void QueryActions::StablishActionError_(Functions::Action& action, std::string error)
{
    action.set_error(true);
    action.set_custom_error(error);
}
