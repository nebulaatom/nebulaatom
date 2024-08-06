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

using namespace NAF::Handlers;

BackendHandler::~BackendHandler()
{

}

void BackendHandler::Process_()
{
    ProcessActions_();
}

void BackendHandler::ProcessActions_()
{
    // Verify current function
        if(get_current_function()->get_actions().empty())
        {
            JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Current function has no actions.");
            return;
        }

    // Setup shared results

        for(auto& action : get_current_function()->get_actions())
        {
            shared_results_.push_back(action->get_results());
        }

    // Process actions of the function
        Tools::OutputLogger::Debug_("Function: " + get_current_function()->get_endpoint());
        JSON::Object::Ptr json_result = new JSON::Object();
        for(auto& action : get_current_function()->get_actions())
        {
            Tools::OutputLogger::Debug_("Action: " + action->get_identifier() + ", Final: " + std::to_string(action->get_final()));

            // Set JSON body
            action->set_json_array(get_json_array());

            // Copy actions
            action->get_actions().clear();
            auto& actions = get_current_function()->get_actions();
            action->get_actions().insert(action->get_actions().end(), actions.begin(), actions.end());
            
            // Identify parameters
            IdentifyParameters_(action);
            if(action->get_error())
            {
                JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action->get_custom_error());
                return;
            }

            // Execute action
            if(!action->Work_())
            {
                JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action->get_custom_error());
                return;
            }

            // Set JSON results
            if(action->get_final())
                json_result = action->get_json_result();

        }

    // Send results
        CompoundResponse_(HTTP::Status::kHTTP_OK, json_result);
}
