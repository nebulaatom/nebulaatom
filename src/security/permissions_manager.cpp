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
            std::list<PermissionToLoad> permissions_to_load;
            auto result_json = query_manager.get_result_json();
            auto& fields = query_manager.get_current_filters_()->get_fields_filter()->get_filter_elements();
            auto& joins = query_manager.get_current_filters_()->get_join_filter()->get_filter_elements();
            auto& general = query_manager.get_current_filters_()->get_general_filter()->get_filter_elements();

        // Reset filters
            query_manager.ResetFilters_();

        // Add filters
            general.insert({"0", {Tools::RowValueFormatter{std::string("up")}, Filters::GeneralFilterElement::Type::kAs}});
            fields.push_back({"tp.table_name", "", "no-quotes"});
            fields.push_back({"tp.route", "", "no-quotes"});
            fields.push_back({"u.username", "", "no-quotes"});
            fields.push_back({"u.id", "", "no-quotes"});
            fields.push_back({"at.name", "", "no-quotes"});
            fields.push_back({"up.granted", "", "no-quotes"});
            fields.push_back({"up.descendant", "", "no-quotes"});
            joins.push_back({"_woodpecker_tables_permissions", "tp", {{"tp.id", {"up.id_table_permission", false}}}, "inner"});
            joins.push_back({"_woodpecker_users", "u", {{"u.id", {"up.id_user", false}}}, "inner"});
            joins.push_back({"_woodpecker_action_types", "at", {{"at.id", {"up.id_action_type", false}}}, "inner"});

        // Execute the query
            if(!query_manager.ComposeQuery_(Query::TypeAction::kSelect, "_woodpecker_user_permissions"))
                return;
            query_manager.get_query()->reset(*query_manager.get_session());
            *query_manager.get_query() << query_manager.get_final_query() , into(permissions_to_load);
            if(!query_manager.ExecuteQuery_())
                return;

        // Iterate over the results
            for(auto it : permissions_to_load)
            {
                // Verify and assign fields
                std::string table_name, route, user, action_type;
                bool granted, descendant;
                int id;
                ActionType action_mapped = ActionType::kRead;

                // Get elements
                table_name = it.get<0>();
                route = it.get<1>();
                user = it.get<2>();
                id = it.get<3>();
                action_type = it.get<4>();
                granted = it.get<5>();
                descendant = it.get<6>();

                // Create permission
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
