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
#include <exception>

using namespace CPW;
using namespace CPW::Tools;

Functions::FunctionsManager SettingsManager::functions_manager_ = {};
SettingsManager::BasicProperties SettingsManager::basic_properties_ = {};

SettingsManager::SettingsManager()
{
}

void SettingsManager::ReadFunctions_()
{
    try
    {
        // Read YAML functions file
        YAML::Node config = YAML::LoadFile("functions.yaml");

        if (!config["functions"] || !config["functions"].IsMap())
        {
            std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML001." << std::endl;
            return;
        }

        // Functions
        for(YAML::const_iterator it = config["functions"].begin(); it != config["functions"].end(); ++it)
        {
            Functions::Function function{"", Functions::Function::Type::kGET};

            // Endpoint
            if (!it->second["endpoint"] || !it->second["endpoint"].IsScalar())
            {
                std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML002." << std::endl;
                return;
            }
            function.set_endpoint(it->second["endpoint"].as<std::string>());

            // Endpoint 2
            if (!it->second["endpoint2"] || !it->second["endpoint2"].IsScalar())
            {
                std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML003." << std::endl;
                return;
            }
            function.set_endpoint2(it->second["endpoint2"].as<std::string>());

            // Target
            if (!it->second["target"] || !it->second["target"].IsScalar())
            {
                std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML004." << std::endl;
                return;
            }
            function.set_target(it->second["target"].as<std::string>());

            // Function Type
            if (!it->second["type"] || !it->second["type"].IsScalar())
            {
                std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML005." << std::endl;
                return;
            }

            auto found_function_type = function.get_methods().find(it->second["type"].as<std::string>());
            if(found_function_type != function.get_methods().end())
                function.set_type(found_function_type->second);

            // Actions
            if (!it->second["actions"] || !it->second["actions"].IsMap())
            {
                std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML006." << std::endl;
                return;
            }

            auto actions = it->second["actions"];
            for(YAML::const_iterator it2 = actions.begin(); it2 != actions.end(); ++it2)
            {
                Functions::Action action{it2->first.as<std::string>()};

                // Basic actions properties verification

                if (!it2->second["customError"] || !it2->second["customError"].IsScalar())
                {
                    std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML007." << std::endl;
                    return;
                }
                if (!it2->second["sqlCode"] || !it2->second["sqlCode"].IsScalar())
                {
                    std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML008." << std::endl;
                    return;
                }
                if (!it2->second["final"] || !it2->second["final"].IsScalar())
                {
                    std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML009." << std::endl;
                    return;
                }

                action.set_custom_error(it2->second["customError"].as<std::string>());
                action.set_final(it2->second["final"].as<bool>());
                action.set_sql_code(it2->second["sqlCode"].as<std::string>());

                // Parameters
                auto parameters = it2->second["parameters"];
                for(YAML::const_iterator it3 = parameters.begin(); it3 != parameters.end(); ++it3)
                {
                    Query::Parameter parameter{it3->first.as<std::string>(), Tools::RowValueFormatter{}, false};

                    // Basic parameter properties

                    if (!it3->second["type"] || !it3->second["type"].IsScalar())
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML010." << std::endl;
                        return;
                    }
                    if (!it3->second["value"])
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML011." << std::endl;
                        return;
                    }
                    if (!it3->second["editable"] || !it3->second["editable"].IsScalar())
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML012." << std::endl;
                        return;
                    }

                    parameter.set_editable(it3->second["editable"].as<bool>());

                    // Parameter value and type
                    auto parameter_type = it3->second["type"].as<std::string>();
                    auto parameter_value = it3->second["value"];
                    if(parameter_type == "conditional")
                    {
                        parameter.set_parameter_type(Query::ParameterType::kConditional);

                        // Basic parameter type/value properties
                        if(!parameter_value["row"] || !parameter_value["row"].IsScalar())
                        {
                            std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML013." << std::endl;
                            return;
                        }
                        if(!parameter_value["column"] || !parameter_value["column"].IsScalar())
                        {
                            std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML014." << std::endl;
                            return;
                        }
                        if(!parameter_value["action_results"] || !parameter_value["action_results"].IsScalar())
                        {
                            std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML015." << std::endl;
                            return;
                        }

                        parameter.get_conditional_field().set_row(parameter_value["row"].as<int>());
                        parameter.get_conditional_field().set_column(parameter_value["column"].as<int>());
                        

                        // Parameter action results
                        auto action_found = std::find_if(function.get_actions().begin(), function.get_actions().end(),[&parameter_value](Functions::Action& action)
                        {
                            return action.get_identifier() == parameter_value["action_results"].as<std::string>();
                        });

                        if(action_found == function.get_actions().end())
                        {
                            std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML016." << std::endl;
                            return;
                        }
                        
                        parameter.set_conditional_field_action(parameter_value["action_results"].as<std::string>());

                    }
                    else if(parameter_type == "field")
                    {
                        parameter.set_parameter_type(Query::ParameterType::kField);

                        if(parameter_value["string"] && parameter_value["string"].IsScalar())
                            parameter.set_value(Tools::RowValueFormatter{parameter_value["string"].as<std::string>()});
                        else if(parameter_value["int"] && parameter_value["int"].IsScalar())
                            parameter.set_value(Tools::RowValueFormatter{parameter_value["int"].as<int>()});
                        else if(parameter_value["float"] && parameter_value["float"].IsScalar())
                            parameter.set_value(Tools::RowValueFormatter{parameter_value["float"].as<float>()});
                        else
                        {
                            std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML017." << std::endl;
                            return;
                        }
                    }
                    else
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML017." << std::endl;
                        return;
                    }
                    
                    action.get_parameters().push_back(std::move(parameter));
                }

                // Conditions
                auto conditions = it2->second["conditions"];
                for(YAML::const_iterator it4 = conditions.begin(); it4 != conditions.end(); ++it4)
                {
                    auto condition = Query::Condition{Query::ConditionType::kGreatherThan, Tools::RowValueFormatter{}, Query::ConditionalField{0, 0}};

                    // Basic condition properties
                    if (!it4->second["type"] || !it4->second["type"].IsScalar())
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML018." << std::endl;
                        return;
                    }
                    if (!it4->second["value"])
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML019." << std::endl;
                        return;
                    }
                    if (!it4->second["conditionalField"]["row"] || !it4->second["conditionalField"]["row"].IsScalar())
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML020." << std::endl;
                        return;
                    }
                    if (!it4->second["conditionalField"]["column"] || !it4->second["conditionalField"]["column"].IsScalar())
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML021." << std::endl;
                        return;
                    }

                    auto condition_type = it4->second["type"].as<std::string>();
                    if(condition_type == "Iqual") condition.set_type(Query::ConditionType::kIqual);
                    else if(condition_type == "NoIqual") condition.set_type(Query::ConditionType::kNoIqual);
                    else if(condition_type == "GreatherThan") condition.set_type(Query::ConditionType::kGreatherThan);
                    else if(condition_type == "SmallerThan") condition.set_type(Query::ConditionType::kSmallerThan);
                    else if(condition_type == "List") condition.set_type(Query::ConditionType::kList);
                    else
                    {
                        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML022." << std::endl;
                        return;
                    }

                    // Value
                    if(condition_type == "List")
                    {
                        auto values = it4->second["value"];

                        for(YAML::const_iterator it5 = values.begin(); it5 != values.end(); ++it5)
                        {
                            if(it5->first.as<std::string>() == "string")
                                condition.get_row_values().push_back(Tools::RowValueFormatter{it5->second.as<std::string>()});
                            else if(it5->first.as<std::string>() == "int")
                                condition.get_row_values().push_back(Tools::RowValueFormatter{it5->second.as<int>()});
                            else if(it5->first.as<std::string>() == "float")
                                condition.get_row_values().push_back(Tools::RowValueFormatter{it5->second.as<float>()});
                            else
                            {
                                std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML023." << std::endl;
                                return;
                            }
                        }
                    }
                    else
                    {
                        auto value = it4->second["value"];

                        if(value["string"])
                            condition.set_row_value(Tools::RowValueFormatter{value["string"].as<std::string>()});
                        else if(value["int"])
                            condition.set_row_value(Tools::RowValueFormatter{value["int"].as<int>()});
                        else if(value["float"])
                            condition.set_row_value(Tools::RowValueFormatter{value["float"].as<float>()});
                        else
                        {
                            std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): The functions.yaml file is malformed. ERRYML024." << std::endl;
                            return;
                        }
                    }

                    condition.get_conditional_field().set_row(it4->second["conditionalField"]["row"].as<int>());
                    condition.get_conditional_field().set_column(it4->second["conditionalField"]["column"].as<int>());

                    action.get_conditions().push_back(std::move(condition));
                }

                // Save action
                function.get_actions().push_back(std::move(action));
            }

            // Save the function
            auto endpoint = it->second["endpoint"].as<std::string>();
            functions_manager_.get_functions().emplace(std::make_pair(endpoint, std::move(function)));
        }
    }
    catch(std::exception& e)
    {
        std::cout << "- Error on settings_manager.cpp on ReadFunctions_(): " << e.what() << std::endl;
        return;
    }
}

void SettingsManager::ReadBasicProperties_()
{
    try
    {
        basic_properties_.port = 80;
        basic_properties_.max_queued = 100;
        basic_properties_.max_threads = 16;
        basic_properties_.db_host = "";
        basic_properties_.db_port = "";
        basic_properties_.db_name = "";
        basic_properties_.db_user = "";
        basic_properties_.db_password = "";
        
        // Read YAML functions file
        YAML::Node config = YAML::LoadFile("properties.yaml");

        // Port
        if (config["port"] && config["port"].IsScalar())
        {
            basic_properties_.port = config["port"].as<int>();
        }
        // Max Queued
        if (config["max_queued"] && config["max_queued"].IsScalar())
        {
            basic_properties_.max_queued = config["max_queued"].as<int>();
        }
        // Max threads
        if (config["max_threads"] && config["max_threads"].IsScalar())
        {
            basic_properties_.max_threads = config["max_threads"].as<int>();
        }
        // DB Host
        if (config["db_host"] && config["db_host"].IsScalar())
        {
            basic_properties_.db_host = config["db_host"].as<std::string>();
        }
        // DB Port
        if (config["db_port"] && config["db_port"].IsScalar())
        {
            basic_properties_.db_port = config["db_port"].as<std::string>();
        }
        // DB Name
        if (config["db_name"] && config["db_name"].IsScalar())
        {
            basic_properties_.db_name = config["db_name"].as<std::string>();
        }
        // DB User
        if (config["db_user"] && config["db_user"].IsScalar())
        {
            basic_properties_.db_user = config["db_user"].as<std::string>();
        }
        // DB Password
        if (config["db_password"] && config["db_password"].IsScalar())
        {
            basic_properties_.db_password = config["db_password"].as<std::string>();
        }
    }
    catch(std::exception& e)
    {
        std::cout << "- Error on settings_manager.cpp on ReadBasicProperties_(): " << e.what() << std::endl;
        return;
    }
}