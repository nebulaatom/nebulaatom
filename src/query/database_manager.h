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

#ifndef CPW_QUERY_DATABASE_MANAGER_H
#define CPW_QUERY_DATABASE_MANAGER_H


#include <iostream>
#include <memory>
#include <string>

#include "Poco/Exception.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>


namespace CPW
{
    namespace Query
    {
        class DatabaseManager;
    }
}

using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;


class CPW::Query::DatabaseManager
{
    public:
        DatabaseManager();
        ~DatabaseManager();

        std::shared_ptr<Data::Session> get_data_session() const
        {
            return data_session_;
        }

        void StartMySQL_();

    private:
        std::shared_ptr<Data::Session> data_session_;

};

#endif // CPW_QUERY_DATABASE_MANAGER_H
