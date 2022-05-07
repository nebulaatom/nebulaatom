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
        auto json_auth = query_actions->get_json_body()->getArray("pair-information")->getObject(0)->getObject("auth");
        auto& iquals = query_actions->get_current_filters_()->get_iquals_conditions();

    // Verify the key-values
        if(json_auth->get("user").isEmpty() || json_auth->get("password").isEmpty())
        {
            iquals.emplace(std::make_pair("username", "null"));
            iquals.emplace(std::make_pair("password", "null"));
        }
        else
        {
            iquals.emplace(std::make_pair("username", json_auth->get("user").toString()));
            iquals.emplace(std::make_pair("password", json_auth->get("password").toString()));
        }

    // Execute the query
        query_actions->ComposeQuery_(Core::TypeAction::kSelect, "users");
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
        auto& iquals = query_actions->get_current_filters_()->get_iquals_conditions();
        int granted = -1;

    // Find user
        std::string user;
        auto found = iquals.find("username");
        if(found != iquals.end())
            user = found->second.get_value();
        else
            user = "null";

    // Verify permissions for the users
        for(auto it : std::vector{user, std::string("null")})
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
        fields.push_back({"pl.granted", false});
        joins.emplace(std::make_pair
        (
            std::array<std::string, 2>{"LEFT", "permissions p"}
            ,std::map<std::string, Extras::ValuesProperties> {{"p.id", Extras::ValuesProperties{"pl.id_permission", false}}}
        ));
        joins.emplace(std::make_pair
        (
            std::array<std::string, 2>{"LEFT", "users u"}
            ,std::map<std::string, Extras::ValuesProperties> {{"u.id", Extras::ValuesProperties{"pl.id_user", false}}}
        ));
        iquals.emplace(std::make_pair("u.username", Extras::ValuesProperties{user, true}));
        iquals.emplace(std::make_pair("pl.type", Extras::ValuesProperties{action_type, true}));
        iquals.emplace(std::make_pair("p.name", Extras::ValuesProperties{target, true}));

    // Data sentences
        query_actions->ComposeQuery_(Core::TypeAction::kSelect, "permissions_log pl");
        if(query_actions->ExecuteQuery_())
            return true;
        else
            return false;
}
