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
#include "query/parameter.h"

using namespace NAF::Security;

UsersManager::UsersManager()
{
    // Setting up the action
    action_ = std::make_shared<Functions::Action>("login-action");
    action_->set_sql_code("SELECT id FROM _naf_users WHERE username = ? AND password = ?");
    action_->AddParameter_("username", Tools::DValue(""), true);
    action_->AddParameter_("password", Tools::DValue(""), true);
}

bool UsersManager::AuthenticateUser_()
{
    try
    {
        // Set current user
            auto& parameters = action_->get_parameters();
            auto found = std::find_if(parameters.begin(), parameters.end(), [&](Query::Parameter::Ptr parameter)
            {
                return parameter->get_name() == "username";
            });
            if(found != parameters.end())
                current_user_.set_username((*found)->ToString_());

        // Query process
            action_->ComposeQuery_();
            if(action_->get_error())
                return false;
            action_->ExecuteQuery_();
            if(action_->get_error())
                return false;
            action_->MakeResults_();

        // Verify results
            if(action_->get_results()->size() > 0)
                return true;
            else
                return false;
    }
    catch(const std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on users_manager.cpp on AuthenticateUser_(): " + std::string(error.what()));
        return false;
    }
}
