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
    // Identify parameters
    IdentifyParameters_();

    // Copy files parameters
    auto& files = get_files_parameters()->get_files();
    auto& file_manager = get_current_function()->get_file_manager();
    for(auto it = files.begin(); it != files.end(); ++it)
        file_manager->get_files().push_back(*it);

    // Set current user
    auto id = get_users_manager().get_current_user().get_id();
    auto username = get_users_manager().get_current_user().get_username();
    auto id_group = get_users_manager().get_current_user().get_id_group();

    get_current_function()->get_current_user().set_id(id);
    get_current_function()->get_current_user().set_username(username);
    get_current_function()->get_current_user().set_id_group(id_group);

    // Process current function
    get_current_function()->Process_(get_http_server_request(), get_http_server_response());
}
