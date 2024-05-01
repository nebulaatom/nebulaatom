
#include "functions/action.h"
#include "query/database_manager.h"
#include "tools/settings_manager.h"

using namespace Atom;
using namespace Atom::Functions;

Action::Action(std::string identifier) :
    identifier_(identifier)
    ,status_("OK.")
    ,message_("OK.")
    ,custom_error_("")
    ,final_(false)
    ,error_(false)
    ,sql_code_("SELECT 1")
    ,final_query_("")
    ,affected_rows_(0)
{
    results_ = std::make_shared<Query::Results>();
    json_result_ = new JSON::Object;
}

Action::~Action()
{

}


void Action::IdentifyParameters_()
{
    try
    {
        auto& data_array = get_json_body();

        for (std::size_t a = 0; a < data_array->size(); a++)
        {
            // Get the action object
                if(data_array->get(a).isEmpty())
                {
                    Tools::OutputLogger::Log_("Data array haves an empty action.");
                    continue;
                }

                auto action_object = data_array->getObject(a);

            // Get the action identifier
                if(action_object->get("action_id").isEmpty() || !action_object->get("action_id").isString())
                {
                    Tools::OutputLogger::Log_("The action object does not have an action_id String Object.");
                    continue;
                }

                auto action_id = action_object->get("action_id").toString();

            // Verify Action identifier
                if(action_id != get_identifier())
                    continue;

            // Get the parameters object
                if(action_object->get("parameters").isEmpty() || !action_object->get("parameters").isArray())
                {
                    Tools::OutputLogger::Log_("The action object does not have a parameters array.");
                    continue;
                }

                auto parameters_array = action_object->getArray("parameters");

            // Iterate over parameters array
                for(std::size_t b = 0; b < parameters_array->size(); b++)
                {
                    // Get parameter object
                    if(parameters_array->get(b).isEmpty())
                    {
                        Tools::OutputLogger::Log_("Parameters array haves an empty element.");
                        continue;
                    }

                    auto parameter_object = parameters_array->getObject(b);

                    // Get parameter name
                    if(parameter_object->get("name").isEmpty() || !parameter_object->get("name").isString())
                    {
                        Tools::OutputLogger::Log_("Parameter name is not a String Object.");
                        continue;
                    }

                    auto parameter_name = parameter_object->get("name").toString();

                    // Get parameter value
                    if(parameter_object->get("value").isEmpty())
                    {
                        Tools::OutputLogger::Log_("Parameter value is empty.");
                        continue;
                    }

                    auto parameter_value = parameter_object->get("value");

                    // Find if exists parameter
                    auto found_param = std::find_if(get_parameters().begin(), get_parameters().end(), [parameter_name](Query::Parameter parameter)
                    {
                        return parameter.get_name() == parameter_name;
                    });

                    // Remplace parameter value
                    if(found_param != get_parameters().end())
                    {
                        if(found_param->get_editable())
                        {
                            auto index = std::distance(get_parameters().begin(), found_param);
                            get_parameters().erase(found_param);


void Action::IdentifyParameters_(JSON::Array::Ptr json_array)
{
    try
    {
        // Iterate over JSON array
        for (std::size_t a = 0; a < json_array->size(); a++)
        {
            // Get Parameters Array
                JSON::Array::Ptr parameters_array = GetParametersArray_(json_array, a);
                if(parameters_array == nullptr)
                    continue;

            // Iterate over parameters array
                for(std::size_t b = 0; b < parameters_array->size(); b++)
                {
                    // Get parameter object
                    Query::Parameter::Ptr parameter = GetParameterObject_(parameters_array, b);
                    if(parameter == nullptr)
                        continue;

                    // Replace Parameter
                    ReplaceParamater_(parameter);
                }

        }
    }
    catch(JSON::JSONException& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
}

void Action::IdentifyParameters_(std::shared_ptr<Net::HTMLForm> form)
{
    try
    {
        // Iterate over files
        for (auto& value : *form)
        {
            // Get parameter object
            Query::Parameter::Ptr parameter(new Query::Parameter(value.first, value.second, true));

            // Replace Parameter
            ReplaceParamater_(parameter);
        }
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
}

void Action::IdentifyParameters_(Files::FileManager& files_parameters)
{
    try
    {
        // Iterate over files
        for (auto& file : files_parameters.get_files())
        {
            float filesize = file.get_tmp_file()->getSize();
            if(filesize > Tools::SettingsManager::get_basic_properties_().max_file_size * 1000000)
                continue;

            std::ifstream istr;
            std::stringstream parameter_value;
            istr.open(file.get_tmp_file()->path());
            StreamCopier::copyStream(istr, parameter_value);
            istr.close();

            // Get parameter object
            Query::Parameter::Ptr parameter(new Query::Parameter(file.get_name(), parameter_value.str(), true));

            // Replace Parameter
            ReplaceParamater_(parameter);
        }
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
}

void Action::IdentifyParameters_(URI::QueryParameters& query_parameters)
{
    try
    {
        // Iterate over JSON array
        for (auto& query_parameter : query_parameters)
        {
            // Get parameter object
            Query::Parameter::Ptr parameter(new Query::Parameter(query_parameter.first, query_parameter.second, true));

            // Replace Parameter
            ReplaceParamater_(parameter);
        }
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on IdentifyFilters_(): " + std::string(error.what()));
    }
}

Query::Parameter& Action::AddParameter_(std::string name, Tools::DValue value, bool editable)
{
    parameters_.push_back(Query::Parameter{name, value, editable});
    return parameters_.back();
}

Query::Parameter& Action::AddParameter_(std::string name, Query::Field::Position field_position, std::string related_action, bool editable)
{
    parameters_.push_back(Query::Parameter{name, field_position, related_action, editable});
    return parameters_.back();
}

Query::Condition& Action::AddCondition_(std::string identifier, Query::ConditionType type, Query::Condition::Functor functor)
{
    Query::Condition condition(identifier, type, functor);
    conditions_.push_back(condition);
    return conditions_.back();
}

bool Action::Work_()
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

bool Action::ComposeQuery_()
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
                switch(param.get_value().get_type())
                {
                    case Tools::DValue::Type::kEmpty:
                    {
                        auto value = Poco::Nullable<int>();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::DValue::Type::kString:
                    {
                        auto& value = param.get_value().get_value_string();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::DValue::Type::kInteger:
                    {
                        auto& value = param.get_value().get_value_int();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::DValue::Type::kFloat:
                    {
                        auto& value = param.get_value().get_value_float();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::DValue::Type::kBoolean:
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

void Action::ExecuteQuery_()
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

void Action::MakeResults_()
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

JSON::Object::Ptr Action::CreateJSONResult_()
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
                    switch(row_value.get_type())
                    {
                        case Tools::DValue::Type::kBoolean:
                            row_fields->set(results.columnName(a), row_value.get_value_bool());
                            break;
                        case Tools::DValue::Type::kFloat:
                            row_fields->set(results.columnName(a), row_value.get_value_float());
                            break;
                        case Tools::DValue::Type::kInteger:
                            row_fields->set(results.columnName(a), row_value.get_value_int());
                            break;
                        case Tools::DValue::Type::kString:
                            row_fields->set(results.columnName(a), row_value.get_value_string());
                            break;
                        case Tools::DValue::Type::kEmpty:
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

bool Action::InitializeQuery_()
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