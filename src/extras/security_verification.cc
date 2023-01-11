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

#include "extras/security_verification.h"

using namespace CPW::Extras;

SecurityVerification::SecurityVerification()
{

}

SecurityVerification::~SecurityVerification()
{

}

bool SecurityVerification::AuthenticateUser_()
{
    // Variables
        auto query_actions = dynamic_elements_.get_query_actions();
        auto result_json = query_actions->get_result_json();
        auto& iquals = query_actions->get_current_filters_()->get_iquals_conditions();

    // Add user and password
        iquals.emplace(std::make_pair("username", user_));
        iquals.emplace(std::make_pair("password", password_));

    // Execute the query
        query_actions->ComposeQuery_(Query::TypeAction::kSelect, "users");
        if(!query_actions->ExecuteQuery_())
            return false;

        auto results = result_json->getArray("results");

        if(results->size() > 0)
            return true;
        else
            return false;
}

bool SecurityVerification::VerifyPermissions_(std::string method)
{
    // Variables
        auto query_actions = dynamic_elements_.get_query_actions();
        auto result_json = query_actions->get_result_json();
        std::string target = dynamic_elements_.get_requested_route()->get_target();
        int granted = -1;

    // Verify permissions for the users
        for(auto it : std::vector{user_, std::string("null")})
        {
            if(!SeePermissionsPerUser_(it, method, target))
                return false;

            if(result_json->get("results").isEmpty())
                continue;

            auto results_array = result_json->getArray("results");

            if(results_array->size() < 1)
                continue;
            if(results_array->getArray(0)->size() < 1)
                continue;
            if(results_array->getArray(0)->get(0).isEmpty())
                continue;
            if(!results_array->getArray(0)->get(0).isInteger())
                continue;

            granted = std::stoi(results_array->getArray(0)->get(0).toString());

            return granted == 1 ? true : false;
        }

        return false;
}

bool SecurityVerification::SeePermissionsPerUser_(std::string user, std::string action_type, std::string target)
{
    // Variables
        auto query_actions = dynamic_elements_.get_query_actions();
        auto& iquals = query_actions->get_current_filters_()->get_iquals_conditions();
        auto& fields = query_actions->get_current_filters_()->get_fields();
        auto& joins = query_actions->get_current_filters_()->get_joins();

    // Clear previous values
        iquals.clear();
        fields.clear();
        joins.clear();

    // Filters
        fields.push_back({"up.granted", false});
        joins.emplace(std::make_pair
        (
            std::array<std::string, 2>{"LEFT", "tables_permissions tp"}
            ,std::map<std::string, Extras::ValuesProperties> {{"tp.id", Extras::ValuesProperties{"up.id_table_permission", false}}}
        ));
        joins.emplace(std::make_pair
        (
            std::array<std::string, 2>{"LEFT", "users u"}
            ,std::map<std::string, Extras::ValuesProperties> {{"u.id", Extras::ValuesProperties{"up.id_user", false}}}
        ));
        joins.emplace(std::make_pair
        (
            std::array<std::string, 2>{"LEFT", "action_types at"}
            ,std::map<std::string, Extras::ValuesProperties> {{"at.id", Extras::ValuesProperties{"up.id_action_type", false}}}
        ));
        iquals.emplace(std::make_pair("u.username", Extras::ValuesProperties{user, true}));
        iquals.emplace(std::make_pair("at.name", Extras::ValuesProperties{action_type, true}));
        iquals.emplace(std::make_pair("tp.table_name", Extras::ValuesProperties{target, true}));

    // Data sentences
        query_actions->ComposeQuery_(Query::TypeAction::kSelect, "user_permissions up");
        if(query_actions->ExecuteQuery_())
            return true;
        else
            return false;
}
