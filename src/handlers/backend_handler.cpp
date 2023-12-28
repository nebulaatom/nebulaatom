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
#include "functions/action.h"
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
    // Read YAML functions file
    YAML::Node config = YAML::LoadFile("functions.yaml");

    if (!config["functions"] || !config["functions"].IsMap())
    {
        app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML001.");
        return;
    }

    // Functions
    for(YAML::const_iterator it = config["functions"].begin(); it != config["functions"].end(); ++it)
    {
        Functions::Function function{"", Functions::Function::Type::kGET};

        // Endpoint
        if (!it->second["endpoint"] || !it->second["endpoint"].IsScalar())
        {
            app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML002.");
            return;
        }
        function.set_endpoint(it->second["endpoint"].as<std::string>());

        // Endpoint 2
        if (!it->second["endpoint2"] || !it->second["endpoint2"].IsScalar())
        {
            app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML003.");
            return;
        }

        // Target
        if (!it->second["target"] || !it->second["target"].IsScalar())
        {
            app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML004.");
            return;
        }

        // Function Type
        if (!it->second["type"] || !it->second["type"].IsScalar())
        {
            app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML005.");
            return;
        }

        auto found_function_type = function.get_methods().find(it->second["type"].as<std::string>());
        if(found_function_type != function.get_methods().end())
            function.set_type(found_function_type->second);

        // Actions
        if (!it->second["actions"] || !it->second["actions"].IsMap())
        {
            app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML006.");
            return;
        }

        auto actions = it->second["actions"];
        for(YAML::const_iterator it2 = actions.begin(); it2 != actions.end(); ++it2)
        {
            Functions::Action action{it2->first.as<std::string>()};

            // Basic actions properties verification

            if (!it2->second["customError"] || !it2->second["customError"].IsScalar())
            {
                app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML007.");
                return;
            }
            if (!it2->second["sqlCode"] || !it2->second["sqlCode"].IsScalar())
            {
                app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML008.");
                return;
            }
            if (!it2->second["final"] || !it2->second["final"].IsScalar())
            {
                app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML009.");
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
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML010.");
                    return;
                }
                if (!it3->second["value"])
                {
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML011.");
                    return;
                }
                if (!it3->second["editable"] || !it3->second["editable"].IsScalar())
                {
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML012.");
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
                        app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML013.");
                        return;
                    }
                    if(!parameter_value["column"] || !parameter_value["column"].IsScalar())
                    {
                        app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML014.");
                        return;
                    }
                    if(!parameter_value["action_results"] || !parameter_value["action_results"].IsScalar())
                    {
                        app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML015.");
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
                        app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML016.");
                        return;
                    }

                    auto& param_result = parameter.get_result();
                    auto action_result = action_found->get_results();
                    param_result = action_result;

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
                        app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML017.");
                        return;
                    }
                }
                else
                {
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML017.");
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
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML018.");
                    return;
                }
                if (!it4->second["value"])
                {
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML019.");
                    return;
                }
                if (!it4->second["conditionalField"]["row"] || !it4->second["conditionalField"]["row"].IsScalar())
                {
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML020.");
                    return;
                }
                if (!it4->second["conditionalField"]["column"] || !it4->second["conditionalField"]["column"].IsScalar())
                {
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML021.");
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
                    app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML022.");
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
                            app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML023.");
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
                        app_.logger().error("- Error on backend_handler.cpp on AddRoutes_(): The functions.yaml file is malformed. ERRYML024.");
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
