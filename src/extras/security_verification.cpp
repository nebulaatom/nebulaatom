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

SecurityVerification::SecurityVerification():
    authenticated_(false)
    ,enabled_(false)
    ,user_("")
    ,password_("")
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
        query_actions->ComposeQuery_(Query::TypeAction::kSelect, "_woodpecker_users");
        if(!query_actions->ExecuteQuery_())
            return authenticated_ = false;

        auto results = result_json->getArray("results");

        if(results->size() > 0)
            return authenticated_ = true;
        else
            return authenticated_ = false;

}

bool SecurityVerification::VerifyPermissions_(Extras::SecurityType security_type, std::string method)
{
    // Variables
        auto query_actions = dynamic_elements_.get_query_actions();
        auto result_json = query_actions->get_result_json();

    // Verify permissions for the targets
        AddTargets_();
        for(auto target : targets_)
        {
            // Verify permissions for the users
            for(auto user : std::vector{user_, std::string("null")})
            {
                bool enabled_for_this_user = false;
                switch(security_type)
                {
                    case SecurityType::kDisableAll:
                    default:
                    {
                        if(!SeePermissionsPerUserTarget_(user, method, target))
                            return enabled_ = false;

                        if(!VerifyPermissionIsOK_(result_json))
                            return enabled_ = false;

                        if((enabled_for_this_user = VerifyPermissionGranted_(result_json)))
                            break;
                        else
                            return enabled_ = false;

                        break;
                    }
                    case SecurityType::kEnableAll:
                    {
                        if(!SeePermissionsPerUserTarget_(user, method, target))
                        {
                            enabled_ = true;
                            continue;
                        }

                        if(!VerifyPermissionIsOK_(result_json))
                        {
                            enabled_ = true;
                            continue;
                        }

                        enabled_ = VerifyPermissionGranted_(result_json);

                        break;
                    }
                }

                if(enabled_for_this_user)
                {
                    enabled_ = true;
                    break;
                }
            }
        }

        return enabled_;
}

void SecurityVerification::AddTargets_()
{
    dynamic_elements_.get_query_actions()->IdentifyFilters_();
    auto& joins = dynamic_elements_.get_query_actions()->get_current_filters_()->get_joins();

    for(auto it = joins.begin(); it != joins.end(); it++)
    {
        if(it->first.size() != 2)
            continue;

        targets_.push_back(it->first[1]);
    }
    dynamic_elements_.get_query_actions()->ResetFilters_();

    std::string target = dynamic_elements_.get_requested_route()->get_target();
    targets_.push_back(target);
}

bool SecurityVerification::VerifyPermissionIsOK_(JSON::Object::Ptr result_json)
{
    if(result_json->get("results").isEmpty())
        return false;

    auto results_array = result_json->getArray("results");
    if(results_array->size() < 1)
        return false;
    if(results_array->getArray(0)->size() < 1)
        return false;
    if(results_array->getArray(0)->get(0).isEmpty())
        return false;
    if(!results_array->getArray(0)->get(0).isInteger())
        return false;

    return true;
}

bool SecurityVerification::VerifyPermissionGranted_(JSON::Object::Ptr result_json)
{
    auto results_array = result_json->getArray("results");
    int granted = -1;
    granted = std::stoi(results_array->getArray(0)->get(0).toString());

    return granted == 1 ? true : false;
}

bool SecurityVerification::SeePermissionsPerUserTarget_(std::string user, std::string action_type, std::string target)
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
        joins.push_back(std::make_pair
        (
            std::array<std::string, 2>{"LEFT", "_woodpecker_tables_permissions tp"}
            ,std::map<std::string, Extras::ValuesProperties> {{"tp.id", Extras::ValuesProperties{"up.id_table_permission", false}}}
        ));
        joins.push_back(std::make_pair
        (
            std::array<std::string, 2>{"LEFT", "_woodpecker_users u"}
            ,std::map<std::string, Extras::ValuesProperties> {{"u.id", Extras::ValuesProperties{"up.id_user", false}}}
        ));
        joins.push_back(std::make_pair
        (
            std::array<std::string, 2>{"LEFT", "_woodpecker_action_types at"}
            ,std::map<std::string, Extras::ValuesProperties> {{"at.id", Extras::ValuesProperties{"up.id_action_type", false}}}
        ));
        iquals.emplace(std::make_pair("u.username", Extras::ValuesProperties{user, true}));
        iquals.emplace(std::make_pair("at.name", Extras::ValuesProperties{action_type, true}));
        iquals.emplace(std::make_pair("tp.table_name", Extras::ValuesProperties{target, true}));

    // Data sentences
        query_actions->ComposeQuery_(Query::TypeAction::kSelect, "_woodpecker_user_permissions up");
        if(query_actions->ExecuteQuery_())
            return true;
        else
            return false;
}
