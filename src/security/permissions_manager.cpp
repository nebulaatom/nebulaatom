/*
* <one line to give the program's name and a brief idea of what it does.>
* Copyright (C) 2021  <copyright holder> <email>
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

#include "security/permissions_manager.h"

using namespace Atom;
using namespace Atom::Security;

std::mutex PermissionsManager::mutex_;
std::list<Permission> PermissionsManager::permissions_ = {};
std::map<std::string, ActionType> PermissionsManager::action_type_map_ = {};

PermissionsManager::PermissionsManager()
{
    
}

std::_List_iterator<Permission> PermissionsManager::FindPermission_(Tools::Route& route, std::string user, std::string action_type)
{
    auto permission_final = permissions_.end();

    ActionType action_mapped = ActionType::kRead;
    auto action_found = action_type_map_.find(action_type);
    if(action_found != action_type_map_.end())
        action_mapped = action_found->second;

    auto permission_found = std::find_if(permissions_.begin(), permissions_.end(), [&](Permission& permission)
    {
        return permission.get_route() == route
        && permission.get_user()->get_username() == user
        && permission.get_action_type() == action_mapped;
    });

    if (permission_found != permissions_.end())
    {
        permission_final = permission_found;
    }

    return permission_final;
}

void PermissionsManager::FillActionTypeMap_()
{
    action_type_map_.clear();
    action_type_map_.insert({"POST", ActionType::kCreate});
    action_type_map_.insert({"GET", ActionType::kRead});
    action_type_map_.insert({"PUT", ActionType::kUpdate});
    action_type_map_.insert({"DEL", ActionType::kDelete});
}

void PermissionsManager::LoadPermissions_()
{
    try
    {
        mutex_.lock();
        FillActionTypeMap_();
        if(permissions_.size() > 0)
        {
            return;
        }

        // Setting up the action
            Functions::Action action{""};
            action.set_custom_error("Permissions not found.");
            std::string sql_code =
                "SELECT ap.endpoint AS endpoint, au.username AS username, au.id AS id_user, ap.action AS action "
                "FROM _atom_permissions ap "
                "JOIN _atom_users au ON au.id = ap.id_user"
            ;
            action.set_sql_code(sql_code);

        // Query process
            action.ComposeQuery_();
            if(action.get_error())
                return;
            action.ExecuteQuery_();
            if(action.get_error())
                return;
            action.MakeResults_();

        // Iterate over the results
            for(auto& row : *action.get_results())
            {
                // Get elements
                auto endpoint = row->FindField_("endpoint");
                auto username = row->FindField_("username");
                auto id_user = row->FindField_("id_user");
                auto action = row->FindField_("action");

                if(endpoint == nullptr || username == nullptr || id_user == nullptr || action == nullptr)
                {
                    throw std::runtime_error("Error to get results, FindField_ return a nullptr object.");
                    return;
                }

                // Create permission
                ActionType action_mapped = ActionType::kRead;
                auto found = action_type_map_.find(action->String_());
                if(found != action_type_map_.end())
                    action_mapped = found->second;

                Permission p
                {
                    Tools::Route{endpoint->String_()}
                    ,std::make_shared<User>(id_user->Int_(), username->String_(), ""), action_mapped
                };

                permissions_.push_back(std::move(p));
            }
        mutex_.unlock();
    }
    catch(const std::exception& error)
    {
        std::cerr << "- Error on PermissionsManager::LoadPermissions_(): " << std::string(error.what()) << std::endl;
        return;
    }
}
