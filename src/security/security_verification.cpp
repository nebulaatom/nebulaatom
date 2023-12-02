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
        /*// Variables
            auto result_json = query_manager_.get_result_json();
            auto& general = query_manager_.get_current_filters_()->get_general_filter()->get_filter_elements();
            auto& fields = query_manager_.get_current_filters_()->get_fields_filter()->get_filter_elements();
            auto& list = query_manager_.get_current_filters_()->get_list_filter()->get_filter_elements();

        // Add filters
            query_manager_.ResetFilters_();
            general.insert({"0", {Tools::RowValueFormatter{std::string("tp")}, Filters::GeneralFilterElement::Type::kAs}});
            fields.push_back({"tp.table_name"});
            fields.push_back({"tp.route"});

            std::list<Tools::RowValueFormatter> values;
            for(auto& target : targets)
            {
                auto target_var = Dynamic::Var{target};
                values.push_back(target_var);
            }

            list.push_back({"tp.table_name", std::move(values), "in"});

        // Execute the query
            if(!query_manager_.ComposeQuery_(Query::TypeAction::kSelect, "_woodpecker_tables_permissions"))
                return;
            *query_manager_.get_query() , into(targets_);
            if(!query_manager_.ExecuteQuery_())
                return;*/

        // Variables
            Query::QueryActions query_manager;

        // Setting up the action
            Functions::Action action{""};
            action.set_custom_error("Permissions not found.");
            std::string sql_code =
                "SELECT tp.table_name, tp.route "
                "FROM _woodpecker_tables_permissions tp "
                "WHERE tp.table_name IN ("
            ;

            // Add targets
            auto targets_size = targets.size();
            for(std::size_t i = 0; i < targets_size; i++)
            {
                if(i != targets_size - 1)
                    sql_code += "?, ";
                else
                    sql_code += "?)";
            }

            action.set_sql_code(sql_code);

            // Parameters
                for(auto& target : targets)
                    action.get_parameters().push_back(Query::Parameter{"available", Tools::RowValueFormatter{target}});

        // Execute de query
            Query::QueryActions query_actions;
            //query_actions.IdentifyParameters_(action);
            query_actions.ComposeQuery_(action);
            query_actions.ExecuteQuery_(action);
            auto results = query_actions.MakeResults_(action);

        // Iterate over the results
            for(auto& row : results.get_rows())
            {
                // Verify and assign fields
                auto table_name = row.FindField_("tp.table_name").get_value().get_value_string();
                auto route_string = row.FindField_("tp.route").get_value().get_value_string();

                // Create route
                Tools::Route route {table_name, route_string};
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
