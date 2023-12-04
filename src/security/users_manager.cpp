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

#include "security/users_manager.h"

using namespace CPW::Security;

UsersManager::UsersManager()
{

}

bool UsersManager::AuthenticateUser_()
{
    try
    {
        /*// Variables
            int count = 0;
            Dynamic::Var username(current_user_.get_username());
            Dynamic::Var password(current_user_.get_password());
            auto result_json = query_manager_.get_result_json();
            auto& fields = query_manager_.get_current_filters_()->get_fields_filter()->get_filter_elements();
            auto& iquals = query_manager_.get_current_filters_()->get_iquals_filter()->get_filter_elements();

        // Add filters
            query_manager_.ResetFilters_();
            fields.push_back({"COUNT(1)"});
            iquals.insert({"0", {"username", username, Filters::IqualsFilterElement::Type::kIqual}});
            iquals.insert({"1", {"password", password, Filters::IqualsFilterElement::Type::kIqual}});

        // Execute the query
            if(!query_manager_.ComposeQuery_(Query::TypeAction::kSelect, "_woodpecker_users"))
                return false;
            *query_manager_.get_query() , into(count);
            if(!query_manager_.ExecuteQuery_())
                return false;
            */

        // Variables
            Query::QueryActions query_manager;
        // Setting up the action
            Functions::Action action{""};
            action.set_custom_error("User not found.");
            std::string sql_code =
                "SELECT id "
                "FROM _woodpecker_users "
                "WHERE username = ? AND password = ?"
            ;
            action.set_sql_code(sql_code);
            action.get_parameters().push_back(Query::Parameter{"username", Tools::RowValueFormatter{current_user_.get_username()}});
            action.get_parameters().push_back(Query::Parameter{"password", Tools::RowValueFormatter{current_user_.get_password()}});

        // Execute de query
            Query::QueryActions query_actions;
            //query_actions.IdentifyParameters_(action);
            query_actions.ComposeQuery_(action);
            if(action.get_error())
                return false;
            query_actions.ExecuteQuery_(action);
            if(action.get_error())
                return false;
            auto results = query_actions.MakeResults_(action);

        // Verify results
            if(results.get_rows().size() > 0)
                return true;
            else
                return false;
    }
    catch(const std::exception& error)
    {
        std::cerr << "- Error on UsersManager::AuthenticateUser_(): " << std::string(error.what());
        return false;
    }
}
