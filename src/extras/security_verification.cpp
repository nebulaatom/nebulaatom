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
    try
    {
        // Variables
            auto query_actions = dynamic_elements_.get_query_actions();
            auto result_json = query_actions->get_result_json();
            auto& iquals = query_actions->get_current_filters_()->get_iquals_filter()->get_filter_elements();

        // Add user and password

            iquals.push_back({"username", {user_, true}, "iqual-quotes"});
            iquals.push_back({"password", {password_, true}, "iqual-quotes"});

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
    catch(const std::exception& error)
    {
        std::cerr << "- Error on SecurityVerification::AuthenticateUser_(): " << std::string(error.what());
        return false;
    }
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
    auto& join = dynamic_elements_.get_query_actions()->get_current_filters_()->get_join_filter()->get_filter_elements();

    for(auto it = join.begin(); it != join.end(); it++)
    {
        targets_.push_back(it->get_table());
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
        auto& iquals = query_actions->get_current_filters_()->get_iquals_filter()->get_filter_elements();
        auto& fields = query_actions->get_current_filters_()->get_fields_filter()->get_filter_elements();
        auto& join = query_actions->get_current_filters_()->get_join_filter()->get_filter_elements();

    // Clear previous values
        iquals.clear();
        fields.clear();
        join.clear();

    // Filters
        fields.push_back({"up.granted", "", "no-quotes"});

        join.push_back({"_woodpecker_tables_permissions", "tp", {{"tp.id", "up.id_table_permission"}}, "left"});
        join.push_back({"_woodpecker_users", "u", {{"u.id", "up.id_user"}}, "left"});
        join.push_back({"_woodpecker_action_types", "at", {{"at.id", "up.id_action_type"}}, "left"});

        iquals.push_back({"u.username", {user, true}, "iqual-quotes"});
        iquals.push_back({"at.name", {action_type, true}, "iqual-quotes"});
        iquals.push_back({"tp.table_name", {target, true}, "iqual-quotes"});

    // Data sentences
        query_actions->ComposeQuery_(Query::TypeAction::kSelect, "_woodpecker_user_permissions up");
        if(query_actions->ExecuteQuery_())
            return true;
        else
            return false;
}
