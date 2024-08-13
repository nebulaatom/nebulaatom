/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2023  Jose F Rivas C <email>
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

#include "tools/sessions_manager.h"

using namespace NAF::Tools;

std::map<std::string, NAF::Extras::Session> SessionsManager::sessions_ = {};
std::mutex SessionsManager::mutex_;
NAF::Query::DatabaseManager::Credentials SessionsManager::credentials_ = Query::DatabaseManager::Credentials
(
    Tools::SettingsManager::GetSetting_("db_host", "localhost")
    ,Tools::SettingsManager::GetSetting_("db_port", "3306")
    ,Tools::SettingsManager::GetSetting_("db_name", "db")
    ,Tools::SettingsManager::GetSetting_("db_user", "root")
    ,Tools::SettingsManager::GetSetting_("db_password", "root")
);

SessionsManager::SessionsManager()
{

}

SessionsManager::~SessionsManager()
{

}

void SessionsManager::ReadSessions_()
{
    try
    {
        mutex_.lock();
        // Setting up the action
            Functions::Action action{""};
            action.get_credentials().Replace_(credentials_);
            action.set_custom_error("Sessions not found.");
            action.set_sql_code("SELECT * FROM _naf_sessions WHERE NOW() < reg_date + INTERVAL max_age SECOND");

        // Query process
            action.ComposeQuery_();
            if(action.get_error())
            {
                Tools::OutputLogger::Error_("Error on sessions_manager.cpp on ReadSessions_(): " + action.get_custom_error());
                return;
            }
            action.ExecuteQuery_();
            if(action.get_error())
            {
                Tools::OutputLogger::Error_("Error on sessions_manager.cpp on ReadSessions_(): " + action.get_custom_error());
                return;
            }
            action.MakeResults_();
            if(action.get_error())
            {
                Tools::OutputLogger::Error_("Error on sessions_manager.cpp on ReadSessions_(): " + action.get_custom_error());
                return;
            }

            for(auto& row : *action.get_results())
            {
                auto identifier = row->ExtractField_("identifier");
                auto path = row->ExtractField_("path");
                auto user = row->ExtractField_("user");
                auto max_age = row->ExtractField_("max_age");

                if(identifier->IsNull_() || path->IsNull_() || user->IsNull_() || max_age->IsNull_())
                {
                    throw std::runtime_error("Error to get results, ExtractField_ return a nullptr object.");
                    return;
                }

                Extras::Session new_session;
                new_session.set_id(identifier->String_());
                new_session.set_user(user->String_());
                new_session.set_path(path->String_());
                new_session.set_max_age(max_age->Int_());

                sessions_.emplace(identifier->String_(), std::move(new_session));

            }
        mutex_.unlock();
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::Error_("Error on sessions_manager.cpp on ReadSessions_(): " + std::string(error.message()));
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on sessions_manager.cpp on ReadSessions_(): " + std::string(error.what()));
    }
}

NAF::Extras::Session& SessionsManager::CreateSession_(std::string user, std::string path, int max_age)
{
    Extras::Session new_session;
    new_session.set_user(user);
    new_session.set_path(path);
    new_session.set_max_age(max_age);

    std::string id;
    while(true)
    {
        id = new_session.get_id();
        if(SessionExists_(id))
            new_session.GenerateNewSessionID_();
        else
            break;
    }

    sessions_.emplace(id, std::move(new_session));

    // Save in DB
    try
    {
        // Setting up the action
            Functions::Action action{""};
            action.get_credentials().Replace_(credentials_);
            action.set_custom_error("Session not saved.");
            std::string sql_code =
                "INSERT INTO _naf_sessions (identifier, path, user, max_age) "
                "VALUES (?, ?, ?, ?)"
            ;
            action.set_sql_code(sql_code);
            action.AddParameter_("identifier", Tools::DValue(id), false);
            action.AddParameter_("path", Tools::DValue(path), false);
            action.AddParameter_("user", Tools::DValue(user), false);
            action.AddParameter_("max_age", Tools::DValue(max_age), false);

        // Query process
            action.ComposeQuery_();
            if(action.get_error())
                return sessions_.at(id);
            action.ExecuteQuery_();
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::Error_("Error on sessions_manager.cpp on CreateSession_(): " + std::string(error.message()));
    }

    return sessions_.at(id);
}

void SessionsManager::DeleteSession_(std::string id)
{
    if(sessions_.find(id) == sessions_.end())
        return;

    sessions_.erase(id);

    try
    {
        // Setting up the action
            Functions::Action action{""};
            action.get_credentials().Replace_(credentials_);
            action.set_custom_error("Session not saved.");
            std::string sql_code =
                "DELETE FROM _woodpecker_sessions WHERE identifier = ?"
            ;
            action.set_sql_code(sql_code);
            action.AddParameter_("identifier", Tools::DValue(id), false);

        // Query process
            action.ComposeQuery_();
            if(action.get_error())
                return;
            action.ExecuteQuery_();
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::Error_("Error on sessions_manager.cpp on DeleteSession_(): " + std::string(error.message()));
    }
}

bool SessionsManager::SessionExists_(std::string id)
{
    return sessions_.find(id) != sessions_.end();
}
