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

using namespace CPW;
using namespace CPW::Security;

std::list<Permission> PermissionsManager::permissions_ = {};
std::map<std::string, ActionType> PermissionsManager::action_type_map_ = {};

PermissionsManager::PermissionsManager()
{
    LoadPermissions_();
}

std::_List_iterator<Permission> PermissionsManager::FindPermission_(Tools::Route& requested_route)
{
    auto found_permission = permissions_.end();
    std::string current_path = "";

    for (const auto& char_segment : requested_route.SegmentsToString_())
    {
        current_path = current_path + char_segment;

        auto found = std::find_if(permissions_.begin(), permissions_.end(), [&current_path](Permission& permission)
        {
            auto string_route = permission.get_route().SegmentsToString_();
            return string_route == current_path;
        });

        if (found != permissions_.end())
        {
            found_permission = found;
        }
    }

    return found_permission;
}

bool PermissionsManager::VerifyPermission_(Tools::Route& requested_route, User& user, std::_List_iterator<Permission> found_permission)
{
    if(found_permission == permissions_.end())
        return false;

    // Verify if are same users
    if(user.get_username() != found_permission->get_user()->get_username())
        return false;

    // Verify if are same route
    bool same_route = false;
    if(requested_route.SegmentsToString_() == found_permission->get_route().SegmentsToString_())
        same_route = true;

    // Get granted and descendant
    bool granted = found_permission->get_granted();
    bool descendant = found_permission->get_descendant();

    // Verify permissions
    if(same_route)
    {
        return granted;
    }
    else
    {
        if(granted && descendant)
            return true;
        else
            return false;
    }
}

void PermissionsManager::FillActionTypeMap_()
{
    action_type_map_.insert({"POST", ActionType::kCreate});
    action_type_map_.insert({"GET", ActionType::kRead});
    action_type_map_.insert({"PUT", ActionType::kUpdate});
    action_type_map_.insert({"DEL", ActionType::kDelete});
}

void PermissionsManager::LoadPermissions_()
{
    try
    {
        if(permissions_.size() > 0)
        {
            return;
        }

        // Variables
            Query::QueryActions query_manager;

        // Setting up the action
            Functions::Action action{""};
            action.set_custom_error("Permissions not found.");
            std::string sql_code =
                "SELECT tp.table_name, tp.route, u.username, u.id, at.name, up.granted, up.descendant "
                "FROM _woodpecker_user_permissions up "
                "JOIN _woodpecker_tables_permissions tp ON tp.id = up.id_table_permission "
                "JOIN _woodpecker_users u ON u.id = up.id_user "
                "JOIN _woodpecker_action_types at ON at.id = up.id_action_type"
            ;
            action.set_sql_code(sql_code);

        // Execute de query
            Query::QueryActions query_actions;
            //query_actions.IdentifyParameters_(action);
            query_actions.ComposeQuery_(action);
            query_actions.ExecuteQuery_(action);
            auto results = query_actions.MakeResults_(action);

        // Iterate over the results
            for(auto& row : results.get_rows())
            {
                // Get elements
                auto table_name = row.FindField_("tp.table_name").get_value().get_value_string();
                auto route = row.FindField_("tp.route").get_value().get_value_string();
                auto user = row.FindField_("u.username").get_value().get_value_string();
                auto id = row.FindField_("u.id").get_value().get_value_int();
                auto action_type = row.FindField_("at.name").get_value().get_value_string();
                auto granted = row.FindField_("up.granted").get_value().get_value_bool();
                auto descendant = row.FindField_("up.descendant").get_value().get_value_bool();

                // Create permission
                ActionType action_mapped = ActionType::kRead;
                auto found = action_type_map_.find(action_type);
                if(found != action_type_map_.end())
                    action_mapped = found->second;

                Permission p {granted, {table_name, route}, std::make_shared<User>(id, user, ""), action_mapped, descendant};

                permissions_.push_back(std::move(p));
            }
    }
    catch(const std::exception& error)
    {
        std::cerr << "- Error on PermissionsManager::LoadPermissions_(): " << std::string(error.what()) << std::endl;
        return;
    }
}
