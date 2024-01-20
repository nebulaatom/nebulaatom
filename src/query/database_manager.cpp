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

using namespace CPW;
using namespace CPW::Query;

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{

}

void DatabaseManager::StartMySQL_()
{
    Data::MySQL::Connector::registerConnector();
}

void DatabaseManager::StopMySQL_()
{
    Data::MySQL::Connector::unregisterConnector();
}

std::shared_ptr<Data::Session> DatabaseManager::StartSessionMySQL_()
{
    Tools::SettingsManager settings_manager;

    std::string db_host = settings_manager.get_basic_properties_().db_host;
    std::string db_port = settings_manager.get_basic_properties_().db_port;
    std::string db_name = settings_manager.get_basic_properties_().db_name;
    std::string db_user = settings_manager.get_basic_properties_().db_user;
    std::string db_password = settings_manager.get_basic_properties_().db_password;

    return std::make_shared<Data::Session>
    (
        "MySQL"
        ,"host="+db_host+";port="+db_port+";db="+db_name+";user="+db_user+";password="+db_password+";"
    );
}
