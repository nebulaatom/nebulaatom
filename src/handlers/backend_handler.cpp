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

BackendHandler::BackendHandler() :
    RootHandler::RootHandler()
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
    // Identify parameters
    IdentifyParameters_();

    // Process current function
    JSON::Object::Ptr json_result = new JSON::Object();
    if(!get_current_function()->ProcessJSONResponse_(json_result))
    {
        if(get_current_function()->get_error())
        {
            JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, get_current_function()->get_error_message());
            return;
        }
    }

    // Send JSON results
    CompoundResponse_(HTTP::Status::kHTTP_OK, json_result);
}
