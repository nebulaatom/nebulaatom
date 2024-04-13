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

using namespace Atom::Security;

UsersManager::UsersManager()
{

}

bool UsersManager::AuthenticateUser_()
{
    try
    {
        // Setting up the action
            Functions::SQLAction action{""};
            action.set_custom_error("User not found.");
            std::string sql_code =
                "SELECT id "
                "FROM _atom_users "
                "WHERE username = ? AND password = ?"
            ;
            action.set_sql_code(sql_code);
            action.get_parameters().push_back(Query::Parameter{"username", Tools::RowValueFormatter{current_user_.get_username()}, false});
            action.get_parameters().push_back(Query::Parameter{"password", Tools::RowValueFormatter{current_user_.get_password()}, false});

        // Query process
            action.ComposeQuery_();
            if(action.get_error())
                return false;
            action.ExecuteQuery_();
            if(action.get_error())
                return false;
            action.MakeResults_();

        // Verify results
            if(action.get_results()->get_rows().size() > 0)
                return true;
            else
                return false;
    }
    catch(const std::exception& error)
    {
        Tools::OutputLogger::instance_.Log_("- Error on users_manager.cpp on AuthenticateUser_(): " + std::string(error.what()));
        return false;
    }
}
