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

using namespace NAF::Security;

UsersManager::UsersManager() :
    credentials_
    (
        Tools::SettingsManager::GetSetting_("db_host", "localhost")
        ,Tools::SettingsManager::GetSetting_("db_port", "3306")
        ,Tools::SettingsManager::GetSetting_("db_name", "db")
        ,Tools::SettingsManager::GetSetting_("db_user", "root")
        ,Tools::SettingsManager::GetSetting_("db_password", "root")
    )
{
    // Setting up the action
    action_ = std::make_shared<Functions::Action>("login-action");
    action_->get_credentials().Replace_(credentials_);
    action_->set_sql_code("SELECT id, username, id_group FROM _naf_users WHERE username = ? AND password = ?");
    action_->AddParameter_("username", Tools::DValue(""), true);
    action_->AddParameter_("password", Tools::DValue(""), true);
}

bool UsersManager::AuthenticateUser_()
{
    try
    {
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
            {
                // Set current user
                auto row = action_->get_results()->front();
                auto id = row->ExtractField_("id");
                auto username = row->ExtractField_("username");
                auto id_group = row->ExtractField_("id_group");

                if(id->IsNull_() || username->IsNull_() || id_group->IsNull_())
                {
                    throw std::runtime_error("Error to get results, Null object.");
                    return false;
                }

                current_user_.set_id(id->Int_());
                current_user_.set_username(username->String_());
                current_user_.set_id_group(id_group->Int_());

                return true;
            }
            else
                return false;
    }
    catch(const std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on users_manager.cpp on AuthenticateUser_(): " + std::string(error.what()));
        return false;
    }
}
