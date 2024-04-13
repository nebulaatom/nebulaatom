/*
* Nebula Atom

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

using namespace Atom::Handlers;

BackendHandler::~BackendHandler()
{

}

void BackendHandler::AddFunctions_()
{
    for(auto& function : get_functions_manager().get_functions())
    {
        get_routes_list().push_back(Tools::Route{function.second.get_endpoint2()});
    }
}

void BackendHandler::Process_()
{
    ProcessActions_();
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

void BackendHandler::ProcessActions_()
{
    // Verify current function
        if(get_current_function().get_actions().empty())
        {
            JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Current function has no actions.");
            return;
        }

    // Setup shared results

        for(auto& action : get_current_function().get_actions())
        {
            shared_results_.push_back(action->get_results());
        }

    // Process actions of the function
        Tools::OutputLogger::instance_.Log_("Function: " + get_current_function().get_endpoint());
        JSON::Object::Ptr json_result = new JSON::Object();
        for(auto& action : get_current_function().get_actions())
        {
            Tools::OutputLogger::instance_.Log_("Action: " + action->get_identifier() + ", Final: " + std::to_string(action->get_final()));

            // Set JSNO body
            action->get_json_body().reset(get_json_body());

            // Copy actions
            action->get_actions().clear();
            auto& actions = get_current_function().get_actions();
            action->get_actions().insert(action->get_actions().end(), actions.begin(), actions.end());
            
            switch(action->get_action_type())
            {
                case Functions::ActionType::kSQL:
                {
                    if(!action->Work_())
                    {
                        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, action->get_custom_error());
                        return;
                    }
                    break;
                }
                case Functions::ActionType::kEmail:
                {
                    if(!action->Work_())
                    {
                        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, action->get_custom_error());
                        return;
                    }

                    JSON::Array::Ptr data_array = new JSON::Array();
                    json_result->set("data", data_array);
                    json_result->set("status", "OK.");
                    json_result->set("message", "OK.");
                    break;
                }
            }

            // Set JSON results
            if(action->get_final())
                json_result.reset(action->get_json_result());

        }

    // Send results
        CompoundResponse_(HTTP::Status::kHTTP_OK, json_result);
}