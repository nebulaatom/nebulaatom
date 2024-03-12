/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2023  Jose F Rivas C <email>
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

#include "tools/settings_manager.h"
#include "functions/sql_action.h"

using namespace Atom;
using namespace Tools;

Tools::SettingsManager::BasicProperties Tools::SettingsManager::basic_properties_ = {};

Tools::SettingsManager::SettingsManager()
{
    
}

void Tools::SettingsManager::SetUpProperties_()
{
    basic_properties_.port = 8080;
    basic_properties_.max_queued = 100;
    basic_properties_.max_threads = 16;
    basic_properties_.db_host = "127.0.0.1";
    basic_properties_.db_port = "3306";
    basic_properties_.db_name = "";
    basic_properties_.db_user = "";
    basic_properties_.db_password = "";
    basic_properties_.session_max_age = 3600;
    basic_properties_.directory_base = "/var/www";
    basic_properties_.directory_for_uploaded_files = "/var/www";
    basic_properties_.directory_for_temp_files = "/tmp";
    basic_properties_.certificate = "";
    basic_properties_.key = "";
    basic_properties_.rootcert = "";
}

void Tools::SettingsManager::ReadFunctions_()
{
    /*try
    {
        // Read YAML functions file
        YAML::Node config = YAML::LoadFile("functions.yaml");

        auto functions = config["functions"];
        if (!VerifyYAMLMapNode_(functions))
        {
            PrintError_("ReadFunctions_", "functions");
            return;
        }

        // Functions
        for(YAML::const_iterator it = functions.begin(); it != functions.end(); ++it)
        {
            Functions::Function function{"", Functions::Function::Type::kGET};

            // Endpoint
            auto endpoint = it->second["endpoint"];
            if (!VerifyYAMLScalarNode_(endpoint))
            {
                PrintError_("ReadFunctions_", "Endpoint");
                return;
            }
            function.set_endpoint(endpoint.as<std::string>());

            // Endpoint 2
            auto endpoint2 = it->second["endpoint2"];
            if (!VerifyYAMLScalarNode_(endpoint))
            {
                PrintError_("ReadFunctions_", "endpoint2");
                return;
            }
            function.set_endpoint2(endpoint2.as<std::string>());

            // Target
            auto target = it->second["target"];
            if (!VerifyYAMLScalarNode_(endpoint))
            {
                PrintError_("ReadFunctions_", "target");
                return;
            }
            function.set_target(target.as<std::string>());

            // Function Type
            auto type = it->second["type"];
            if (!VerifyYAMLScalarNode_(type))
            {
                PrintError_("ReadFunctions_", "type");
                return;
            }

            auto found_function_type = function.get_methods().find(type.as<std::string>());
            if(found_function_type != function.get_methods().end())
                function.set_type(found_function_type->second);

            // Actions
            auto actions = it->second["actions"];
            if (!VerifyYAMLMapNode_(actions))
            {
                PrintError_("ReadFunctions_", "actions");
                return;
            }

            for(YAML::const_iterator it2 = actions.begin(); it2 != actions.end(); ++it2)
            {
                Functions::Function::ActionPtr action;

                // customError
                auto custom_error = it2->second["customError"];
                if (!VerifyYAMLScalarNode_(custom_error))
                {
                    PrintError_("ReadFunctions_", "customError");
                    return;
                }
                action->set_custom_error(it2->second["customError"].as<std::string>());

                // final
                auto final = it2->second["final"];
                if (!VerifyYAMLScalarNode_(final))
                {
                    PrintError_("ReadFunctions_", "final");
                    return;
                }
                action->set_final(it2->second["final"].as<bool>());

                // parameters
                auto parameters = it2->second["parameters"];
                ReadFunctionsParameters_(function, action, parameters);

                // Action type
                auto action_type = it2->second["type"];
                if (!VerifyYAMLScalarNode_(action_type))
                {
                    PrintError_("ReadFunctions_", "action_type");
                    return;
                }

                if(action_type.as<std::string>() == "sql")
                {
                    // SQL Action
                    auto action_object = std::make_shared<Functions::SQLAction>(it2->first.as<std::string>());

                        // sqlCode
                        auto sql_code = it2->second["sqlCode"];
                        if (!VerifyYAMLScalarNode_(sql_code))
                        {
                            PrintError_("ReadFunctions_", "sqlCode");
                            return;
                        }
                        action_object->set_sql_code(sql_code.as<std::string>());

                        // conditions
                        auto conditions = it2->second["conditions"];
                        ReadFunctionsConditions_(action_object, conditions);

                    action = action_object;
                }
                else if(it2->second["type"].as<std::string>() == "email")
                {
                    // Email Action
                    auto action_object = std::make_shared<Functions::EmailAction>(it2->first.as<std::string>());

                        // mailHost
                        auto mail_host = it2->second["mailHost"];
                        if (!VerifyYAMLScalarNode_(mail_host))
                        {
                            PrintError_("ReadFunctions_", "mailHost");
                            return;
                        }
                        action_object->set_mail_host(mail_host.as<std::string>());

                        // sender
                        auto sender = it2->second["sender"];
                        if (!VerifyYAMLScalarNode_(sender))
                        {
                            PrintError_("ReadFunctions_", "sender");
                            return;
                        }
                        action_object->set_sender(sender.as<std::string>());

                        // recipient
                        auto recipient = it2->second["recipient"];
                        if (!VerifyYAMLScalarNode_(recipient))
                        {
                            PrintError_("ReadFunctions_", "recipient");
                            return;
                        }
                        action_object->set_recipient(recipient.as<std::string>());

                        // subject
                        auto subject = it2->second["subject"];
                        if (!VerifyYAMLScalarNode_(subject))
                        {
                            PrintError_("ReadFunctions_", "subject");
                            return;
                        }
                        action_object->set_subject(subject.as<std::string>());

                        // message
                        auto message = it2->second["message"];
                        if (!VerifyYAMLScalarNode_(message))
                        {
                            PrintError_("ReadFunctions_", "message");
                            return;
                        }
                        action_object->set_email_message(message.as<std::string>());

                        // emailUser
                        auto email_user = it2->second["emailUser"];
                        if (!VerifyYAMLScalarNode_(email_user))
                        {
                            PrintError_("ReadFunctions_", "emailUser");
                            return;
                        }
                        action_object->set_email_user(email_user.as<std::string>());

                        // emailPassword
                        auto email_password = it2->second["emailPassword"];
                        if (!VerifyYAMLScalarNode_(email_password))
                        {
                            PrintError_("ReadFunctions_", "emailPassword");
                            return;
                        }
                        action_object->set_email_password(email_password.as<std::string>());

                    action = action_object;
                }
                else
                {
                    PrintError_("ReadFunctions_", "action_type");
                    return;
                }

                // Save action
                function.get_actions().push_back(std::move(action));
            }

            // Save the function
            functions_manager_.get_functions().emplace(std::make_pair(endpoint.as<std::string>(), std::move(function)));
        }
    }
    catch(std::exception& e)
    {
        PrintError_("ReadFunctions_", "MSG: " + std::string(e.what()));
        return;
    }*/
}

void Tools::SettingsManager::ReadFunctionsParameters_(Functions::Function& function, Functions::Function::ActionPtr action, YAML::Node& parameters)
{
    // Parameters
    for(YAML::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
    {
        Query::Parameter parameter{it->first.as<std::string>(), Tools::RowValueFormatter{}, false};

        // type
        auto type = it->second["type"];
        if (!VerifyYAMLScalarNode_(type))
        {
            PrintError_("ReadFunctionsParameters_", "type");
            return;
        }

        // editable
        auto editable = it->second["editable"];
        if (!VerifyYAMLScalarNode_(editable))
        {
            PrintError_("ReadFunctionsParameters_", "editable");
            return;
        }
        parameter.set_editable(it->second["editable"].as<bool>());

        // Parameter value and type
        auto value = it->second["value"];
        if (!VerifyYAMLMapNode_(value))
        {
            PrintError_("ReadFunctionsParameters_", "value");
            return;
        }
        
        if(type.as<std::string>() == "conditional")
        {
            parameter.set_parameter_type(Query::ParameterType::kConditional);

            // row
            auto row = value["row"];
            if (!VerifyYAMLScalarNode_(row))
            {
                PrintError_("ReadFunctionsParameters_", "row");
                return;
            }
            parameter.get_conditional_field().set_row(row.as<int>());

            // column
            auto column = value["column"];
            if (!VerifyYAMLScalarNode_(column))
            {
                PrintError_("ReadFunctionsParameters_", "column");
                return;
            }
            parameter.get_conditional_field().set_column(column.as<int>());

            // action_results
            auto action_results = value["action_results"];
            if (!VerifyYAMLScalarNode_(action_results))
            {
                PrintError_("ReadFunctionsParameters_", "action_results");
                return;
            }
            auto action_found = std::find_if(function.get_actions().begin(), function.get_actions().end(),[&action_results](Functions::Function::ActionPtr& action)
            {
                return action->get_identifier() == action_results.as<std::string>();
            });
            if(action_found == function.get_actions().end())
            {
                PrintError_("ReadFunctionsParameters_", "action_results:action_found");
                return;
            }
            parameter.set_conditional_field_action(action_results.as<std::string>());

        }
        else if(type.as<std::string>() == "field")
        {
            parameter.set_parameter_type(Query::ParameterType::kField);

            // value_string
            auto value_string = value["string"];
            if (VerifyYAMLScalarNode_(value_string))
                parameter.set_value(Tools::RowValueFormatter{value_string.as<std::string>()});
            else if (VerifyYAMLScalarNode_(value_string))
                parameter.set_value(Tools::RowValueFormatter{value_string.as<int>()});
            else if (VerifyYAMLScalarNode_(value_string))
                parameter.set_value(Tools::RowValueFormatter{value_string.as<float>()});
            else
            {
                PrintError_("ReadFunctionsParameters_", "value:string-int-float");
                return;
            }
        }
        else
        {
            PrintError_("ReadFunctionsParameters_", "type:value");
            return;
        }
        
        action->get_parameters().push_back(std::move(parameter));
    }
}

void Tools::SettingsManager::ReadFunctionsConditions_(std::shared_ptr<Functions::Action> action, YAML::Node& conditions)
{
    // Conditions
    for(YAML::const_iterator it = conditions.begin(); it != conditions.end(); ++it)
    {
        auto condition = Query::Condition{Query::ConditionType::kGreatherThan, Tools::RowValueFormatter{}, Query::ConditionalField{0, 0}};

        // type
        auto type = it->second["type"];
        if (!VerifyYAMLScalarNode_(type))
        {
            PrintError_("ReadFunctionsConditions_", "type");
            return;
        }
        auto type_string = type.as<std::string>();
        if(type_string == "Iqual") condition.set_type(Query::ConditionType::kIqual);
        else if(type_string == "NoIqual") condition.set_type(Query::ConditionType::kNoIqual);
        else if(type_string == "GreatherThan") condition.set_type(Query::ConditionType::kGreatherThan);
        else if(type_string == "SmallerThan") condition.set_type(Query::ConditionType::kSmallerThan);
        else if(type_string == "List") condition.set_type(Query::ConditionType::kList);
        else
        {
            PrintError_("ReadFunctionsConditions_", "type");
            return;
        }

        // conditionalField-row
        auto conditional_field_row = it->second["conditionalField"]["row"];
        if (!VerifyYAMLScalarNode_(conditional_field_row))
        {
            PrintError_("ReadFunctionsConditions_", "conditionalField-row");
            return;
        }
        condition.get_conditional_field().set_row(conditional_field_row.as<int>());

        // conditionalField-column
        auto conditional_field_column = it->second["conditionalField"]["column"];
        if (!VerifyYAMLScalarNode_(conditional_field_column))
        {
            PrintError_("ReadFunctionsConditions_", "conditionalField-column");
            return;
        }
        condition.get_conditional_field().set_column(conditional_field_column.as<int>());

        // value
        auto value = it->second["value"];
        if (!VerifyYAMLMapNode_(value))
        {
            PrintError_("ReadFunctionsConditions_", "value");
            return;
        }

        if(type_string == "List")
        {
            for(YAML::const_iterator it = value.begin(); it != value.end(); ++it)
            {
                if(!it->IsScalar())
                {
                    PrintError_("ReadFunctionsConditions_", "value");
                    continue;
                }

                if(it->first.as<std::string>() == "string")
                    condition.get_row_values().push_back(Tools::RowValueFormatter{it->second.as<std::string>()});
                else if(it->first.as<std::string>() == "int")
                    condition.get_row_values().push_back(Tools::RowValueFormatter{it->second.as<int>()});
                else if(it->first.as<std::string>() == "float")
                    condition.get_row_values().push_back(Tools::RowValueFormatter{it->second.as<float>()});
                else
                {
                    PrintError_("ReadFunctionsConditions_", "value-list");
                    continue;
                }
            }
        }
        else
        {
            if(!it->IsScalar())
            {
                PrintError_("ReadFunctionsConditions_", "value");
                return;
            }

            if(value["string"])
                condition.set_row_value(Tools::RowValueFormatter{value["string"].as<std::string>()});
            else if(value["int"])
                condition.set_row_value(Tools::RowValueFormatter{value["int"].as<int>()});
            else if(value["float"])
                condition.set_row_value(Tools::RowValueFormatter{value["float"].as<float>()});
            else
            {
                PrintError_("ReadFunctionsConditions_", "value");
                return;
            }
        }

        action->get_conditions().push_back(std::move(condition));
    }
}

void Tools::SettingsManager::ReadBasicProperties_()
{
    try
    {
        // Read YAML functions file
        YAML::Node config = YAML::LoadFile("properties.yaml");

        // port
        auto port = config["port"];
        if (!VerifyYAMLScalarNode_(port))
        {
            PrintError_("ReadBasicProperties_", "port");
            return;
        }
        basic_properties_.port = port.as<int>();
        
        // max_queued
        auto max_queued = config["max_queued"];
        if (!VerifyYAMLScalarNode_(max_queued))
        {
            PrintError_("ReadBasicProperties_", "max_queued");
            return;
        }
        basic_properties_.max_queued = max_queued.as<int>();
        
        // max_threads
        auto max_threads = config["max_threads"];
        if (!VerifyYAMLScalarNode_(max_threads))
        {
            PrintError_("ReadBasicProperties_", "max_threads");
            return;
        }
        basic_properties_.max_threads = max_threads.as<int>();
        
        // session_max_age
        auto session_max_age = config["session_max_age"];
        if (!VerifyYAMLScalarNode_(session_max_age))
        {
            PrintError_("ReadBasicProperties_", "session_max_age");
            return;
        }
        basic_properties_.session_max_age = session_max_age.as<int>();
        
        // directory_base
        auto directory_base = config["directory_base"];
        if (!VerifyYAMLScalarNode_(directory_base))
        {
            PrintError_("ReadBasicProperties_", "directory_base");
            return;
        }
        basic_properties_.directory_base = directory_base.as<std::string>();
        
        // directory_for_uploaded_files
        auto directory_for_uploaded_files = config["directory_for_uploaded_files"];
        if (!VerifyYAMLScalarNode_(directory_for_uploaded_files))
        {
            PrintError_("ReadBasicProperties_", "directory_for_uploaded_files");
            return;
        }
        basic_properties_.directory_for_uploaded_files = directory_for_uploaded_files.as<std::string>();
        
        // directory_for_temp_files
        auto directory_for_temp_files = config["directory_for_temp_files"];
        if (!VerifyYAMLScalarNode_(directory_for_temp_files))
        {
            PrintError_("ReadBasicProperties_", "directory_for_temp_files");
            return;
        }
        basic_properties_.directory_for_temp_files = directory_for_temp_files.as<std::string>();
        
        // db_host
        auto db_host = config["db_host"];
        if (!VerifyYAMLScalarNode_(db_host))
        {
            PrintError_("ReadBasicProperties_", "db_host");
            return;
        }
        basic_properties_.db_host = db_host.as<std::string>();
        
        // db_port
        auto db_port = config["db_port"];
        if (!VerifyYAMLScalarNode_(db_port))
        {
            PrintError_("ReadBasicProperties_", "db_port");
            return;
        }
        basic_properties_.db_port = db_port.as<std::string>();
        
        // db_name
        auto db_name = config["db_name"];
        if (!VerifyYAMLScalarNode_(db_name))
        {
            PrintError_("ReadBasicProperties_", "db_name");
            return;
        }
        basic_properties_.db_name = db_name.as<std::string>();
        
        // db_user
        auto db_user = config["db_user"];
        if (!VerifyYAMLScalarNode_(db_user))
        {
            PrintError_("ReadBasicProperties_", "db_user");
            return;
        }
        basic_properties_.db_user = db_user.as<std::string>();
        
        // db_password
        auto db_password = config["db_password"];
        if (!VerifyYAMLScalarNode_(db_password))
        {
            PrintError_("ReadBasicProperties_", "db_password");
            return;
        }
        basic_properties_.db_password = db_password.as<std::string>();
        
        // certificate
        auto certificate = config["certificate"];
        if (!VerifyYAMLScalarNode_(certificate))
        {
            PrintError_("ReadBasicProperties_", "certificate");
            return;
        }
        basic_properties_.certificate = certificate.as<std::string>();
        
        // key
        auto key = config["key"];
        if (!VerifyYAMLScalarNode_(key))
        {
            PrintError_("ReadBasicProperties_", "key");
            return;
        }
        basic_properties_.key = key.as<std::string>();
        
        // rootcert
        auto rootcert = config["rootcert"];
        if (!VerifyYAMLScalarNode_(rootcert))
        {
            PrintError_("ReadBasicProperties_", "rootcert");
            return;
        }
        basic_properties_.rootcert = rootcert.as<std::string>();
        
    }
    catch(std::exception& e)
    {
        PrintError_("ReadBasicProperties_", "general: " + std::string(e.what()));
        return;
    }
}

bool Tools::SettingsManager::VerifyYAMLScalarNode_(YAML::Node& node)
{
    if(!node || !node.IsScalar())
        return false;
    else
        return true;
}

void Tools::SettingsManager::PrintError_(std::string function, std::string variable)
{
    std::cout << "- Error on settings_manager.cpp on " << function << "(): The yaml file is malformed. ERRYML" << function << "(" << variable << ")" << std::endl;
}

bool Tools::SettingsManager::VerifyYAMLMapNode_(YAML::Node& node)
{
    if(!node || !node.IsMap())
        return false;
    else
        return true;
}