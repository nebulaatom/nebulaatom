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

#include "security/security_verification.h"

using namespace CPW::Extras;

SecurityVerification::SecurityVerification()
{

}

void SecurityVerification::AddTargets_(std::list<std::string>& targets)
{
    try
    {
        // Variables
            auto result_json = query_manager_.get_result_json();
            auto& general = query_manager_.get_current_filters_()->get_general_filter()->get_filter_elements();
            auto& fields = query_manager_.get_current_filters_()->get_fields_filter()->get_filter_elements();
            auto& list = query_manager_.get_current_filters_()->get_list_filter()->get_filter_elements();

        // Add filters
            query_manager_.ResetFilters_();
            general.set_as("tp");
            fields.push_back({"tp.table_name", "" ,"no-quotes"});
            fields.push_back({"tp.route", "" ,"no-quotes"});

            std::list<Extras::ValuesProperties> values;
            for(auto& target : targets)
                values.push_back({target, true});

            list.push_back({"tp.table_name", std::move(values), "in"});

        // Execute the query
            query_manager_.ComposeQuery_(Query::TypeAction::kSelect, "_woodpecker_tables_permissions");
            if(!query_manager_.ExecuteQuery_())
                return;

            auto results = result_json->getArray("results");

        // Iterate over the results
            for(std::size_t it = 0; it < results->size(); it++)
            {
                // Verify row element
                auto row = results->isArray(it) ? results->getArray(it) : nullptr;
                if(row == nullptr)
                    return;

                // Verify and assign fields
                std::string target, route_string;

                target = row->get(0).isEmpty() ? "" : row->get(0).toString();
                route_string = row->get(1).isEmpty() ? "" : row->get(1).toString();

                // Create route
                Tools::Route route {target, route_string};
                routes_to_verify_.push_back(std::move(route));
            }
    }
    catch(const std::exception& error)
    {
        std::cerr << "- SecurityVerification::AddTargets_(): " << std::string(error.what());
        return;
    }
}

bool SecurityVerification::VerifyRoutesPermissions_()
{
    try
    {
        // Iterate over all routes
        for(auto& route : routes_to_verify_)
        {
            auto p = permissions_manager_.FindPermission_(route);
            if(p == permissions_manager_.get_permissions().end())
                continue;

            return permissions_manager_.VerifyPermission_(route, users_manager_.get_current_user(), p);
        }

        // Permission not found
        switch(security_type_)
        {
            case SecurityType::kDisableAll: return false; break;
            case SecurityType::kEnableAll: return true; break;
        }
    }
    catch(const std::exception& error)
    {
        std::cerr << "- SecurityVerification::VerifyRoutesPermissions_(): " << std::string(error.what());
        return false;
    }
    return false;
}
