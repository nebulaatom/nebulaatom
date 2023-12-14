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
#include "query/results.h"

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
                a1.get_parameters().push_back(Query::Parameter{"name", Tools::RowValueFormatter{std::string("")}, true});
            // Conditions
                a1.get_conditions().push_back(Query::Condition{Query::ConditionType::kGreatherThan, Query::Field{"", Tools::RowValueFormatter{0}}, Query::ConditionalField{0, 0}});
            f1.get_actions().push_back(a1);

        // Setting up the actions
            Functions::Action a2{"a2"};
            a2.set_custom_error("Error Action 2");
            a2.set_final(true);
            a2.set_sql_code("SELECT * FROM products WHERE id_store = ? AND price < ?");
            // Parameters
                a2.get_parameters().push_back(Query::Parameter{"id_store", Query::ConditionalField{0, 0}, a1.get_results(), false});
                a2.get_parameters().push_back(Query::Parameter{"price", Tools::RowValueFormatter{100}, true});
            f1.get_actions().push_back(a2);

        // Setting up the function
            get_functions_manager().get_functions().insert({endpoint, std::move(f1)});
            get_routes_list().push_back({"products", "api/products"});

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

            query_actions.IdentifyParameters_(action);
            if(action.get_error())
            {
                GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, action.get_custom_error());
                return;
            }
            query_actions.ComposeQuery_(action);
            if(action.get_error())
            {
                GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, action.get_custom_error());
                return;
            }
            query_actions.ExecuteQuery_(action);
            if(action.get_error())
            {
                GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, action.get_custom_error());
                return;
            }
            // Verify Conditions
            query_actions.MakeResults_(action);
            for(auto& condition : action.get_conditions())
            {
                if(!condition.VerifyCondition_(action.get_results()))
                {
                    GenericResponse_(*get_response(), HTTPResponse::HTTP_BAD_REQUEST, "Condition error.");
                    return;
                }
            }
            //results_.emplace(std::pair{action.get_identifier(), std::move(results)});

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
