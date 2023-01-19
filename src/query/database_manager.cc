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

using namespace CPW::Query;

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{
    if(data_session_ != nullptr)
        data_session_->close();
}

void DatabaseManager::StartMySQL_()
{
    Data::MySQL::Connector::registerConnector();
    data_session_ = std::make_shared<Data::Session>("MySQL", "host=127.0.0.1;port=3306;db=cpw_rabbit;user=root;password=0UHC72zNvywZ;");
}
