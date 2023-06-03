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
        // Variables
            auto result_json = query_manager_.get_result_json();
            auto& iquals = query_manager_.get_current_filters_()->get_iquals_filter()->get_filter_elements();

        // Add filters
            query_manager_.ResetFilters_();
            iquals.push_back({"username", {current_user_.get_username(), true}, "iqual-quotes"});
            iquals.push_back({"password", {current_user_.get_password(), true}, "iqual-quotes"});

        // Execute the query
            query_manager_.ComposeQuery_(Query::TypeAction::kSelect, "_woodpecker_users");
            if(!query_manager_.ExecuteQuery_())
                return false;

            auto results = result_json->getArray("results");

            if(results->size() > 0)
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
