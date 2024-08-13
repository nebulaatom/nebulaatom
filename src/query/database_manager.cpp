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

#include "query/database_manager.h"
#include "tools/settings_manager.h"
#include <Poco/Mutex.h>

using namespace NAF;
using namespace NAF::Query;

std::mutex DatabaseManager::mutex_;
bool DatabaseManager::initialized_ = false;

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{

}

void DatabaseManager::StartMySQL_()
{
    Data::MySQL::Connector::registerConnector();
    initialized_ = true;
}

void DatabaseManager::StopMySQL_()
{
    if(initialized_)
        Data::MySQL::Connector::unregisterConnector();
}

std::shared_ptr<Data::Session> DatabaseManager::StartSessionMySQL_()
{
    mutex_.lock();

    std::string db_host = Tools::SettingsManager::GetSetting_("db_host", "localhost");
    std::string db_port = Tools::SettingsManager::GetSetting_("db_port", "3306");
    std::string db_name = Tools::SettingsManager::GetSetting_("db_name", "db");
    std::string db_user = Tools::SettingsManager::GetSetting_("db_user", "");
    std::string db_password = Tools::SettingsManager::GetSetting_("db_password", "");

    mutex_.unlock();
    
    return std::make_shared<Data::Session>
    (
        "MySQL"
        ,"host="+db_host+";port="+db_port+";db="+db_name+";user="+db_user+";password="+db_password+";"
    );
}
