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

#include "handlers/backend_handler.h"
#include "functions/function.h"
#include "query/condition.h"
#include "query/parameter.h"
#include "query/results.h"
#include "tools/row_value_formatter.h"

using namespace CPW::Handlers;

BackendHandler::~BackendHandler()
{

}

void BackendHandler::AddRoutes_()
{
    /*// Function /api/products
        std::string endpoint = "/api/products";
        Functions::Function f1{endpoint, Functions::Function::Type::kGET};

        // Setting up the actions
            Functions::Action a1{"a1"};
            a1.set_custom_error("Error Action 1.");
            a1.set_sql_code("SELECT id FROM stores WHERE name = ?");
            // Parameters
                a1.get_parameters().push_back(Query::Parameter{"storeName", Tools::RowValueFormatter{std::string("")}, true});
            // Conditions
                a1.get_conditions().push_back(Query::Condition{Query::ConditionType::kGreatherThan, Tools::RowValueFormatter(0), Query::ConditionalField(0, 0)});
            f1.get_actions().push_back(a1);

        // Setting up the actions
            Functions::Action a2{"a2"};
            a2.set_custom_error("Error Action 2");
            a2.set_final(true);
            a2.set_sql_code("SELECT * FROM products WHERE id_store = ? AND price < ?");
            // Parameters
                a2.get_parameters().push_back(Query::Parameter{"idStore", Query::ConditionalField{0, 0}, a1.get_results(), false});
                a2.get_parameters().push_back(Query::Parameter{"productPrice", Tools::RowValueFormatter{100}, true});
            f1.get_actions().push_back(a2);

        // Setting up the function
            get_functions_manager().get_functions().insert({endpoint, std::move(f1)});
            get_routes_list().push_back({"products", "api/products"});
    */
    // Read YAML functions
    YAML::Node config = YAML::LoadFile("functions.yaml");

    if (!config["functions"] || !config["functions"].IsMap())
    {
        std::cout << "The functions.yaml file is malformed. ERRYML001." << std::endl;
        return;
    }

    for(YAML::const_iterator it = config["functions"].begin(); it != config["functions"].end(); ++it)
    {
        // Basic function properties
        if (!it->second["endpoint"] || !it->second["endpoint"].IsScalar())
        {
            std::cout << "The functions.yaml file is malformed. ERRYML002." << std::endl;
            return;
        }
        if (!it->second["endpoint2"] || !it->second["endpoint2"].IsScalar())
        {
            std::cout << "The functions.yaml file is malformed. ERRYML002_2." << std::endl;
            return;
        }
        if (!it->second["target"] || !it->second["target"].IsScalar())
        {
            std::cout << "The functions.yaml file is malformed. ERRYML002_3." << std::endl;
            return;
        }
        if (!it->second["type"] || !it->second["type"].IsScalar())
        {
            std::cout << "The functions.yaml file is malformed. ERRYML003." << std::endl;
            return;
        }
        if (!it->second["actions"] || !it->second["actions"].IsMap())
        {
            std::cout << "The functions.yaml file is malformed. ERRYML004." << std::endl;
            return;
        }

        Functions::Function function{it->second["endpoint"].as<std::string>(), Functions::Function::Type::kGET};

        auto found_function_type = function.get_methods().find(it->second["type"].as<std::string>());
        if(found_function_type != function.get_methods().end())
            function.set_type(found_function_type->second);

        // Actions
        auto actions = it->second["actions"];
        for(YAML::const_iterator it2 = actions.begin(); it2 != actions.end(); ++it2)
        {
            // Basic actions properties
            Functions::Action action{it2->first.as<std::string>()};

            if (!it2->second["customError"] || !it2->second["customError"].IsScalar())
            {
                std::cout << "The functions.yaml file is malformed. ERRYML005." << std::endl;
                return;
            }
            if (!it2->second["sqlCode"] || !it2->second["sqlCode"].IsScalar())
            {
                std::cout << "The functions.yaml file is malformed. ERRYML006." << std::endl;
                return;
            }
            if (!it2->second["final"] || !it2->second["final"].IsScalar())
            {
                std::cout << "The functions.yaml file is malformed. ERRYML007." << std::endl;
                return;
            }

            action.set_custom_error(it2->second["customError"].as<std::string>());
            action.set_final(it2->second["final"].as<bool>());
            action.set_sql_code(it2->second["sqlCode"].as<std::string>());

            // Parameters
            auto parameters = it2->second["parameters"];
            for(YAML::const_iterator it3 = parameters.begin(); it3 != parameters.end(); ++it3)
            {
                // Basic parameter properties
                Query::Parameter parameter{it3->first.as<std::string>(), Tools::RowValueFormatter{}, false};

                if (!it3->second["type"] || !it3->second["type"].IsScalar())
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML008." << std::endl;
                    return;
                }
                if (!it3->second["value"])
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML008_2." << std::endl;
                    return;
                }
                if (!it3->second["editable"] || !it3->second["editable"].IsScalar())
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML009." << std::endl;
                    return;
                }

                auto type = it3->second["type"].as<std::string>();
                auto val = it3->second["value"];
                if(type == "conditional")
                {
                    parameter.set_parameter_type(Query::ParameterType::kConditional);

                    if(!val["row"] || !val["row"].IsScalar())
                        std::cout << "The functions.yaml file is malformed. ERRYML010." << std::endl;
                    if(!val["column"] || !val["column"].IsScalar())
                        std::cout << "The functions.yaml file is malformed. ERRYML011." << std::endl;
                    if(!val["action_results"] || !val["action_results"].IsScalar())
                        std::cout << "The functions.yaml file is malformed. ERRYML011_2." << std::endl;

                    parameter.get_conditional_field().set_row(val["row"].as<int>());
                    parameter.get_conditional_field().set_column(val["column"].as<int>());
                    
                    for(auto action : function.get_actions())
                    {
                        if(action.get_identifier() == val["action_results"].as<std::string>())
                        {
                            auto& param_result = parameter.get_result();
                            auto action_result = action.get_results();
                            param_result = action_result;

                            break;
                        }
                    }
                }
                else if(type == "field")
                {
                    parameter.set_parameter_type(Query::ParameterType::kField);

                    if(val["string"] && val["string"].IsScalar())
                        parameter.set_value(Tools::RowValueFormatter{val["string"].as<std::string>()});
                    else if(val["int"] && val["int"].IsScalar())
                        parameter.set_value(Tools::RowValueFormatter{val["int"].as<int>()});
                    else if(val["float"] && val["float"].IsScalar())
                        parameter.set_value(Tools::RowValueFormatter{val["float"].as<float>()});
                }
                else
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML009_2." << std::endl;
                    return;
                }
                
                parameter.set_editable(it3->second["editable"].as<bool>());

                action.get_parameters().push_back(std::move(parameter));
            }

            // Conditions
            auto conditions = it2->second["conditions"];
            for(YAML::const_iterator it4 = conditions.begin(); it4 != conditions.end(); ++it4)
            {
                // Basic condition properties
                auto condition = Query::Condition{Query::ConditionType::kGreatherThan, Tools::RowValueFormatter{}, Query::ConditionalField(0, 0)};
                if (!it4->second["type"] || !it4->second["type"].IsScalar())
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML013." << std::endl;
                    return;
                }
                if (!it4->second["value"])
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML014." << std::endl;
                    return;
                }
                if (!it4->second["conditionalField"]["row"] || !it4->second["conditionalField"]["column"])
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML015." << std::endl;
                    return;
                }

                auto condition_type = it4->second["type"].as<std::string>();
                if(condition_type == "Iqual") condition.set_type(Query::ConditionType::kIqual);
                else if(condition_type == "NoIqual") condition.set_type(Query::ConditionType::kNoIqual);
                else if(condition_type == "GreatherThan") condition.set_type(Query::ConditionType::kGreatherThan);
                else if(condition_type == "SmallerThan") condition.set_type(Query::ConditionType::kSmallerThan);
                else if(condition_type == "List") condition.set_type(Query::ConditionType::kList);

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
                            std::cout << "The functions.yaml file is malformed. ERRYML015_3." << std::endl;
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
                        std::cout << "The functions.yaml file is malformed. ERRYML015_2." << std::endl;
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
        get_functions_manager().get_functions().insert({it->second["endpoint"].as<std::string>(), std::move(function)});
        get_routes_list().push_back({it->second["target"].as<std::string>(), it->second["endpoint2"].as<std::string>()});
    }
}

void BackendHandler::Process_()
{
    // Verify current function
        if(get_current_function() == nullptr)
        {
            GenericResponse_(*get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Current function is Null Pointer.");
            return;
        }

    // Process actions of the function
        JSON::Object::Ptr json_result = new JSON::Object();
        for(auto& action : get_current_function()->get_actions())
        {
            std::cout << "Function: " << get_current_function()->get_endpoint() << ", Action: " << action.get_identifier() << ", Final: " << action.get_final() << std::endl;
            Query::QueryActions query_actions;
            query_actions.get_json_body().reset(get_json_body());

            // Identify parameters
            query_actions.IdentifyParameters_(action);
            if(action.get_error())
            {
                GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, action.get_custom_error());
                return;
            }

            // Compose query
            query_actions.ComposeQuery_(action);
            if(action.get_error())
            {
                GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, action.get_custom_error());
                return;
            }

            // Execute query
            query_actions.ExecuteQuery_(action);
            if(action.get_error())
            {
                GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, action.get_custom_error());
                return;
            }

            // Make results
            query_actions.MakeResults_(action);
            if(action.get_error())
            {
                GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, action.get_custom_error());
                return;
            }

            // Verify Conditions
            for(auto& condition : action.get_conditions())
            {
                if(!condition.VerifyCondition_(action.get_results()))
                {
                    GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, "Condition error.");
                    return;
                }
            }

            if(action.get_final())
            {
                json_result = query_actions.CreateJSONResult_();
                json_result->set("status", action.get_status());
                json_result->set("message", action.get_message());
            }
        }

    // Send results
        CompoundResponse_
        (
            *get_response()
            ,HTTPResponse::HTTP_OK
            ,json_result
        );
}

void BackendHandler::HandleGETMethod_()
{

}

void BackendHandler::HandlePOSTMethod_()
{

}

void BackendHandler::HandlePUTMethod_()
{

}

void BackendHandler::HandleDELMethod_()
{

}
