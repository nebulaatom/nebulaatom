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

#ifndef NAF_QUERY_DATABASE_MANAGER
#define NAF_QUERY_DATABASE_MANAGER


#include <iostream>
#include <memory>
#include <string>
#include <mutex>

#include "Poco/Exception.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>

//#include "tools/settings_manager.h"


namespace NAF
{
    namespace Query
    {
        class DatabaseManager;
    }
}

using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

class SettingsManager;
class NAF::Query::DatabaseManager
{
    public:
        DatabaseManager();
        ~DatabaseManager();

        static void StartMySQL_();
        static void StopMySQL_();
        static std::shared_ptr<Data::Session> StartSessionMySQL_();

    private:
        static std::mutex mutex_;
        static bool initialized_;

};

#endif // NAF_QUERY_DATABASE_MANAGER
