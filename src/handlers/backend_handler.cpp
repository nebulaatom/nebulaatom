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
#include "tools/route.h"
#include "tools/row_value_formatter.h"

using namespace CPW::Handlers;

BackendHandler::~BackendHandler()
{

}

void BackendHandler::AddRoutes_()
{
    for(auto& function : get_settings_manager().get_functions_manager().get_functions())
    {
        get_routes_list().push_back(Tools::Route{function.second.get_endpoint2()});
    }
}

void BackendHandler::Process_()
{
    // Verify current function
        if(get_current_function().get_actions().empty())
        {
            GenericResponse_(*get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Current function has no actions.");
            return;
        }

    // Process actions of the function
        JSON::Object::Ptr json_result = new JSON::Object();
        for(auto& action : get_current_function().get_actions())
        {
            std::cout << "Function: " << get_current_function().get_endpoint() << ", Action: " << action.get_identifier() << ", Final: " << action.get_final() << std::endl;
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
            query_actions.set_current_function(&get_current_function());
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
