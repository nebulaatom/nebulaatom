
#include "functions/action.h"
#include "query/database_manager.h"
#include "tools/output_logger.h"
#include "tools/settings_manager.h"

using namespace Atom;
using namespace Atom::Functions;

Action::Action(std::string identifier) :
    async_(false)
    ,async_finished_(false)
    ,identifier_(identifier)
    ,status_("OK.")
    ,message_("OK.")
    ,custom_error_("")
    ,final_(true)
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
    // The current object will be destroyed when the 
    // asynchronous function finishes executing
    if(async_)
    {
        while(true)
        {
            if(async_finished_)
                break;
        }
    }
}

JSON::Array::Ptr Action::GetParametersArray_(JSON::Array::Ptr json_array, int counter)
{
    // Get the action object
    if(json_array->get(counter).isEmpty())
    {
        Tools::OutputLogger::Log_("Warning on action.cpp on GetParametersArray_(): Data array haves an empty action.");
        return nullptr;
    }
    auto action_object = json_array->getObject(counter);

    // Get the action identifier
    if(action_object->get("action_id").isEmpty() || !action_object->get("action_id").isString())
    {
        Tools::OutputLogger::Log_("Warning on action.cpp on GetParametersArray_(): The action object does not have an action_id String Object.");
        return nullptr;
    }
    auto action_id = action_object->get("action_id").toString();

    // Verify Action identifier
    if(action_id != get_identifier())
        return nullptr;

    // Get the parameters object
    if(action_object->get("parameters").isEmpty() || !action_object->get("parameters").isArray())
    {
        Tools::OutputLogger::Log_("Warning on action.cpp on GetParametersArray_(): The action object does not have a parameters array.");
        return nullptr;
    }

    return action_object->getArray("parameters");
}

Query::Parameter::Ptr Action::GetParameterObject_(JSON::Array::Ptr parameters_array, int counter)
{
    if(parameters_array->get(counter).isEmpty())
    {
        Tools::OutputLogger::Log_("Warning on action.cpp on GetParameterObject_(): Parameters array haves an empty element.");
        return nullptr;
    }

    auto parameter_object = parameters_array->getObject(counter);

    // Get parameter name
    if(parameter_object->get("name").isEmpty() || !parameter_object->get("name").isString())
    {
        Tools::OutputLogger::Log_("Warning on action.cpp on GetParameterObject_(): Parameter name is not a String Object.");
        return nullptr;
    }

    auto parameter_name = parameter_object->get("name").toString();

    // Get parameter value
    if(parameter_object->get("value").isEmpty())
    {
        Tools::OutputLogger::Log_("Warning on action.cpp on GetParameterObject_(): Parameter value is empty.");
        return nullptr;
    }

    auto parameter_value = parameter_object->get("value");

    // Create and return parameter
    Query::Parameter::Ptr parameter(new Query::Parameter(parameter_name, parameter_value, true));
    return parameter;
}

void Action::ReplaceParamater_(Query::Parameter::Ptr parameter)
{
    // Find if exists parameter
    auto found_param = std::find_if(parameters_.begin(), parameters_.end(), [parameter](Query::Parameter::Ptr& param)
    {
        return param->get_name() == parameter->get_name();
    });

    // Remplace parameter value
    if(found_param != parameters_.end())
    {
        if(!found_param->get()->get_editable()) return;

        found_param->get()->set_value(parameter->get_value());
        /*auto index = std::distance(parameters_.begin(), found_param);
        parameters_.erase(found_param);

        // Insert new element
        parameters_.insert(parameters_.begin() + index, parameter);*/
    }
}

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

Query::Parameter::Ptr Action::AddParameter_(std::string name, Tools::DValue value, bool editable)
{
    parameters_.push_back(std::make_shared<Query::Parameter>(name, value, editable));
    return parameters_.back();
}

Query::Parameter::Ptr Action::AddParameter_(std::string name, Query::Field::Position field_position, std::string related_action, bool editable)
{
    parameters_.push_back(std::make_shared<Query::Parameter>(name, field_position, related_action, editable));
    return parameters_.back();
}

bool Action::Work_()
{
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

    if(get_final())
    {
        json_result_ = CreateJSONResult_();
        json_result_->set("status", get_status());
        json_result_->set("message", get_message());
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
            for(auto& param : parameters_)
            {
                // Setup positional parameter
                SetupPositionParameter_(param);

                // Verify Parameter condition
                if(!VerifyParameterCondition_(param))
                    return false;

                // Add final value to query
                switch(param->get_value().get_type())
                {
                    case Tools::DValue::Type::kEmpty:
                    {
                        auto value = Poco::Nullable<int>();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::DValue::Type::kString:
                    {
                        auto& value = param->get_value().get_value_string();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::DValue::Type::kInteger:
                    {
                        auto& value = param->get_value().get_value_int();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::DValue::Type::kFloat:
                    {
                        auto& value = param->get_value().get_value_float();
                        *query_ , use(value);
                        break;
                    }
                    case Tools::DValue::Type::kBoolean:
                    {
                        auto& value = param->get_value().get_value_bool();
                        *query_ , use(value);
                        break;
                    }
                }
                Tools::OutputLogger::Log_("Parameter in ComposeQuery_(): " + param->get_name() + ": " + param->get_value().ToString_());
            }

        // Return
            Tools::OutputLogger::Log_("Final query: " + query_->toString());
            return true;
    }
    catch(MySQL::MySQLException& error)
    {
        NotifyError_("Error on action.cpp on ComposeQuery_(): " + std::string(error.message()));
        return false;
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on ComposeQuery_(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on ComposeQuery_(): " + std::string(error.what()));
        return false;
    }
    return false;
}

void Action::ExecuteQuery_()
{
    try
    {
        affected_rows_ = query_->execute();
        if(async_) async_finished_ = true;
    }
    catch(MySQL::MySQLException& error)
    {
        NotifyError_("Error on action.cpp on ExecuteQuery_(): " + std::string(error.message()));
        return;
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on ExecuteQuery_(): " + std::string(error.what()));
        return;
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on ExecuteQuery_(): " + std::string(error.what()));
        return;
    }
}

void Action::ExecuteAsyncQuery_()
{
    try
    {
        async_ = true;
        async_finished_ = false;
        std::thread thread_obj(&Action::ExecuteQuery_, this);
        thread_obj.detach();
    }
    catch(MySQL::MySQLException& error)
    {
        NotifyError_("Error on action.cpp on ExecuteAsyncQuery_(): " + std::string(error.message()));
        return;
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on ExecuteAsyncQuery_(): " + std::string(error.what()));
        return;
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on ExecuteAsyncQuery_(): " + std::string(error.what()));
        return;
    }
}

void Action::MakeResults_()
{
    try
    {
        mutex_.lock();

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
        
        mutex_.unlock();
    }
    catch(JSON::JSONException& error)
    {
        NotifyError_("Error on action.cpp on CreateJSONResult_(): " + std::string(error.message()));
        return;
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on CreateJSONResult_(): " + std::string(error.what()));
        return;
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on CreateJSONResult_(): " + std::string(error.what()));
        return;
    }

    return;
}

JSON::Object::Ptr Action::CreateJSONResult_()
{
    try
    {
        mutex_.lock();

        // Variables
            JSON::Object::Ptr result_json = new JSON::Object();
            JSON::Array::Ptr data_array = new JSON::Array();

        // Make JSON data
            for(auto& rows : *results_)
            {
                JSON::Object::Ptr row_fields = new JSON::Object();
                
                // Iterate over rows
                for(auto& field : *rows)
                {
                    auto column_name = field->get_column_name();
                    auto& field_value = field->get_value();
                    switch(field->get_value().get_type())
                    {
                        case Tools::DValue::Type::kBoolean:
                            row_fields->set(column_name, field_value.get_value_bool());
                            break;
                        case Tools::DValue::Type::kFloat:
                            row_fields->set(column_name, field_value.get_value_float());
                            break;
                        case Tools::DValue::Type::kInteger:
                            row_fields->set(column_name, field_value.get_value_int());
                            break;
                        case Tools::DValue::Type::kString:
                            row_fields->set(column_name, field_value.get_value_string());
                            break;
                        case Tools::DValue::Type::kEmpty:
                            row_fields->set(column_name, "");
                            break;
                    }
                }

                data_array->set(data_array->size(), row_fields);
            }

            result_json->set("data", data_array);

            mutex_.unlock();
            return result_json;
    }
    catch(JSON::JSONException& error)
    {
        Tools::OutputLogger::Log_("Error on action.cpp on CreateJSONResult_(): " + std::string(error.message()));
        return JSON::Object::Ptr{};
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Log_("Error on action.cpp on CreateJSONResult_(): " + std::string(error.what()));
        return JSON::Object::Ptr{};
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Log_("Error on action.cpp on CreateJSONResult_(): " + std::string(error.what()));
        return JSON::Object::Ptr{};
    }
}

void Action::NotifyError_(std::string message)
{
    Tools::OutputLogger::Log_(message);
    set_error(true);
    set_custom_error(message);
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
        NotifyError_("Error on action.cpp on InitializeQuery_(): " + std::string(error.message()));
        return false;
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on action.cpp on InitializeQuery_(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on action.cpp on InitializeQuery_(): " + std::string(error.what()));
        return false;
    }
    return false;
}

void Action::SetupPositionParameter_(Query::Parameter::Ptr parameter)
{
    if(parameter->get_parameter_type() == Query::ParameterType::kPosition)
    {
        // Find action results
        auto action_found = std::find_if(actions_.begin(), actions_.end(),[&parameter](Functions::Action::Ptr action)
        {
            return action->get_identifier() == parameter->get_related_action();
        });

        // Set DValue found
        if(action_found != actions_.end())
        {
            auto row_value = action_found->get()->get_results()->FindField_(parameter->get_field_position());
            if(row_value != nullptr)
                parameter->set_value(row_value->get_value());
        }
    }
}

bool Action::VerifyParameterCondition_(Query::Parameter::Ptr parameter)
{
    bool result = true;
    if(parameter->get_condition() == nullptr)
        result = true;
    else if(!parameter->get_condition()->VerifyCondition_(parameter))
    {
        switch(parameter->get_condition()->get_type())
        {
            case Query::ConditionType::kWarning:
            {
                if(parameter->get_error() == "")
                    Tools::OutputLogger::Log_("Parameter condition warning (Identifier: " + parameter->get_condition()->get_identifier() + ", Parameter name: " + parameter->get_name() + ").");
                else
                    Tools::OutputLogger::Log_(parameter->get_error());
                result = true;
                break;
            }
            case Query::ConditionType::kError:
            {
                if(parameter->get_error() == "")
                {
                    Tools::OutputLogger::Log_("Parameter condition error (Identifier: " + parameter->get_condition()->get_identifier() + ", Parameter name: " + parameter->get_name() + ").");
                    set_custom_error("Parameter condition error (Identifier: " + parameter->get_condition()->get_identifier() + ", Parameter name: " + parameter->get_name() + ").");
                }
                else
                {
                    Tools::OutputLogger::Log_(parameter->get_error());
                    set_custom_error(parameter->get_error());
                }
                error_ = true;
                result = false;
                break;
            }
        }
    }
    return result;
}