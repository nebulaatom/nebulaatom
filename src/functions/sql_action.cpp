
#include "functions/sql_action.h"
#include "query/database_manager.h"
#include "tools/settings_manager.h"

using namespace Atom;
using namespace Functions;

Functions::SQLAction::SQLAction(std::string identifier) :
    Action(identifier)
    ,sql_code_("SELECT 1")
    ,final_query_("")
    ,affected_rows_(0)
{
    set_action_type(ActionType::kSQL);
}

bool Functions::SQLAction::Work_()
{
    // Identify parameters
    IdentifyParameters_();
    if(get_error())
        return false;

    // Compose query
    ComposeQuery_();
    if(get_error())
        return false;

    // Execute query
    ExecuteQuery_();
    if(get_error())
        return false;

    // Make results
    MakeResults_();
    if(get_error())
        return false;

    // Verify Conditions
    for(auto& condition : get_conditions())
    {
        if(!condition.VerifyCondition_(get_results()))
        {
            switch(condition.get_type())
            {
                case Query::ConditionType::kWarning:
                {
                    Tools::OutputLogger::Log_("Condition warning (" + condition.get_identifier() + ").");

                    return true;
                    break;
                }
                case Query::ConditionType::kError:
                {
                    set_custom_error("Condition error (" + condition.get_identifier() + ").");
                    return false;
                    break;
                }
            }
        }
    }

    if(get_final())
    {
        get_json_result() = CreateJSONResult_();
        get_json_result()->set("status", get_status());
        get_json_result()->set("message", get_message());
    }

    return true;
}

bool Functions::SQLAction::ComposeQuery_()
{
    try
    {
        // Initialize de query statement
            if(!InitializeQuery_())
                return false;

        // Set the query
            *query_ << sql_code_;

        // Set the parameters
            for(auto& param : get_parameters())
            {
                // Verify conditional parameter
                if(param.get_parameter_type() == Query::ParameterType::kPosition)
                {
                    // Find action results
                    auto action_found = std::find_if(get_actions().begin(), get_actions().end(),[&param](std::shared_ptr<Functions::Action>& action)
                    {
                        return action->get_identifier() == param.get_related_action();
                    });

                    if(action_found != get_actions().end())
                    {
                        auto row_value = action_found->get()->get_results()->FindField_(param.get_field_position());
                        param.set_value(row_value->get_value());
                    }
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
            Tools::OutputLogger::Log_("Final query: " + query_->toString());
            return true;
    }
    catch(MySQL::MySQLException& error)
    {
        std::string string_error = "Error on query_actions.cpp on ComposeQuery_(): " + std::string(error.message());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return false;
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "Error on query_actions.cpp on ComposeQuery_(): " + std::string(error.what());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return false;
    }
    catch(std::exception& error)
    {
        std::string string_error = "Error on query_actions.cpp on ComposeQuery_(): " + std::string(error.what());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return false;
    }
    return false;
}

void Functions::SQLAction::ExecuteQuery_()
{
    try
    {
        affected_rows_ = query_->execute();
    }
    catch(MySQL::MySQLException& error)
    {
        std::string string_error = "Error on query_actions.cpp on ExecuteQuery_(): " + std::string(error.message());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return;
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "Error on query_actions.cpp on ExecuteQuery_(): " + std::string(error.what());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return;
    }
    catch(std::exception& error)
    {
        std::string string_error = "Error on query_actions.cpp on ExecuteQuery_(): " + std::string(error.what());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return;
    }
}

void Functions::SQLAction::MakeResults_()
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
                Query::Row::Ptr row_fields(new Query::Row);

                std::size_t col = 0;
                for(size_t a = 0; a < it.fieldCount(); a++)
                {
                    auto column_name = results_dataquery.columnName(col);
                    auto value = it.get(a);
                    // Create shared Query::Field
                    row_fields->push_back(std::make_shared<Query::Field>(column_name, Tools::DValue(value)));
                    col++;
                }

                get_results()->push_back(std::move(row_fields));
            }

        // Close session
            session_->close();
    }
    catch(JSON::JSONException& error)
    {
        std::string string_error = "Error on query_actions.cpp on CreateJSONResult_(): " + std::string(error.message());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return;
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "Error on query_actions.cpp on CreateJSONResult_(): " + std::string(error.what());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return;
    }
    catch(std::exception& error)
    {
        std::string string_error = "Error on query_actions.cpp on CreateJSONResult_(): " + std::string(error.what());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return;
    }

    return;
}

JSON::Object::Ptr Functions::SQLAction::CreateJSONResult_()
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
                    auto row_value = Tools::DValue(var);
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
        Tools::OutputLogger::Log_("Error on query_actions.cpp on CreateJSONResult_(): " + std::string(error.message()));
        return JSON::Object::Ptr{};
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Log_("Error on query_actions.cpp on CreateJSONResult_(): " + std::string(error.what()));
        return JSON::Object::Ptr{};
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Log_("Error on query_actions.cpp on CreateJSONResult_(): " + std::string(error.what()));
        return JSON::Object::Ptr{};
    }
}

bool Functions::SQLAction::InitializeQuery_()
{
    try
    {
        session_ = Query::DatabaseManager::StartSessionMySQL_();
        if(session_.get() == nullptr)
        {
            throw MySQL::MySQLException("Error to connect to database server.");
            return false;
        }

        query_ = std::make_shared<Data::Statement>(*session_);
        return true;
    }
    catch(MySQL::MySQLException& error)
    {
        std::string string_error = "Error on query_actions.cpp on InitializeQuery_(): " + std::string(error.message());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return false;
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "Error on query_actions.cpp on InitializeQuery_(): " + std::string(error.what());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return false;
    }
    catch(std::exception& error)
    {
        std::string string_error = "Error on query_actions.cpp on InitializeQuery_(): " + std::string(error.what());
        Tools::OutputLogger::Log_(string_error);
        set_error(true);
        set_custom_error(string_error);
        return false;
    }
    return false;
}