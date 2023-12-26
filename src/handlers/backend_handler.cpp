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
#include "query/condition.h"
#include "query/results.h"
#include "tools/row_value_formatter.h"
#include "yaml-cpp/node/type.h"

using namespace CPW::Handlers;

BackendHandler::~BackendHandler()
{

}

void BackendHandler::AddRoutes_()
{
    // Function /api/products
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

    // YAML
    YAML::Node config = YAML::LoadFile("functions.yaml");

    if (!config["functions"])
    {
        std::cout << "The functions.yaml file is malformed. ERRYML001." << std::endl;
        return;
    }

    for(YAML::const_iterator it = config["functions"].begin(); it != config["functions"].end(); ++it)
    {
        // Basic function properties
        std::cout << "-- Function: " << it->first.as<std::string>() << "\n";
        if (!it->second["endpoint"])
        {
            std::cout << "The functions.yaml file is malformed. ERRYML002." << std::endl;
            return;
        }
        if (!it->second["type"])
        {
            std::cout << "The functions.yaml file is malformed. ERRYML003." << std::endl;
            return;
        }
        if (!it->second["actions"])
        {
            std::cout << "The functions.yaml file is malformed. ERRYML004." << std::endl;
            return;
        }
        std::cout << "--- Endpoint: " << it->second["endpoint"].as<std::string>() << "\n";
        std::cout << "--- Type: " << it->second["type"].as<std::string>() << "\n";

        // Actions
        auto actions = it->second["actions"];
        for(YAML::const_iterator it2 = actions.begin(); it2 != actions.end(); ++it2)
        {
            // Basic actions properties
            std::cout << "--- Action: " << it2->first.as<std::string>() << "\n";
            if (!it2->second["customError"])
            {
                std::cout << "The functions.yaml file is malformed. ERRYML005." << std::endl;
                return;
            }
            if (!it2->second["sqlCode"])
            {
                std::cout << "The functions.yaml file is malformed. ERRYML006." << std::endl;
                return;
            }
            if (!it2->second["final"])
            {
                std::cout << "The functions.yaml file is malformed. ERRYML007." << std::endl;
                return;
            }
            std::cout << "---- customError: " << it2->second["customError"].as<std::string>() << "\n";
            std::cout << "---- sqlCode: " << it2->second["sqlCode"].as<std::string>() << "\n";
            std::cout << "---- final: " << it2->second["final"].as<std::string>() << "\n";

            // Parameters
            auto parameters = it2->second["parameters"];
            for(YAML::const_iterator it3 = parameters.begin(); it3 != parameters.end(); ++it3)
            {
                // Basic parameter properties
                std::cout << "---- Parameter: " << it3->first.as<std::string>() << "\n";
                if (!it3->second["value"])
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML008." << std::endl;
                    return;
                }
                if (!it3->second["editable"])
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML009." << std::endl;
                    return;
                }

                // Value
                if(it3->second["value"].IsScalar())
                    std::cout << "----- value: " << it3->second["value"].as<std::string>() << "\n";
                else if(it3->second["value"].IsMap())
                {
                    if(!it3->second["value"]["x"])
                        std::cout << "The functions.yaml file is malformed. ERRYML010." << std::endl;
                    if(!it3->second["value"]["y"])
                        std::cout << "The functions.yaml file is malformed. ERRYML011." << std::endl;

                    std::cout << "----- value x: " << it3->second["value"]["x"].as<std::string>() << "\n";
                    std::cout << "----- value y: " << it3->second["value"]["y"].as<std::string>() << "\n";
                }
                else
                    std::cout << "The functions.yaml file is malformed. ERRYML012." << std::endl;

                std::cout << "----- editable: " << it3->second["editable"].as<std::string>() << "\n";
            }

            // Conditions
            auto conditions = it2->second["conditions"];
            for(YAML::const_iterator it4 = conditions.begin(); it4 != conditions.end(); ++it4)
            {
                // Basic condition properties
                std::cout << "---- Condition: " << it4->first.as<std::string>() << "\n";
                if (!it4->second["type"])
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML013." << std::endl;
                    return;
                }
                if (!it4->second["value"])
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML014." << std::endl;
                    return;
                }
                if (!it4->second["conditionalField"]["x"] || !it4->second["conditionalField"]["y"])
                {
                    std::cout << "The functions.yaml file is malformed. ERRYML015." << std::endl;
                    return;
                }

                // Value
                if(it4->second["value"].IsScalar())
                    std::cout << "----- value: " << it4->second["value"].as<std::string>() << "\n";
                else if(it4->second["value"].IsSequence())
                {
                    auto values = it4->second["value"];

                    for(YAML::const_iterator it5 = values.begin(); it5 != values.end(); ++it5)
                    {
                        if(!it5->IsScalar())
                            std::cout << "The functions.yaml file is malformed. ERRYML016." << std::endl;

                        std::cout << "----- value: " << it5->as<std::string>() << "\n";
                    }

                }
                else
                    std::cout << "The functions.yaml file is malformed. ERRYML017." << std::endl;

                std::cout << "----- type: " << it4->second["type"].as<std::string>() << "\n";
                std::cout << "----- conditionalField x: " << it4->second["conditionalField"]["x"].as<std::string>() << "\n";
                std::cout << "----- conditionalField y: " << it4->second["conditionalField"]["y"].as<std::string>() << "\n";
            }
        }
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
