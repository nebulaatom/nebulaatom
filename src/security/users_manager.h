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


#ifndef USERS_MANAGER
#define USERS_MANAGER


#include <string>

#include "Poco/Tuple.h"

#include "functions/sql_action.h"
#include "security/user.h"
#include "tools/output_logger.h"


namespace Atom
{
    namespace Security
    {
        class UsersManager;
    }
}


class Atom::Security::UsersManager
{
    public:
        UsersManager();

        User& get_current_user()
        {
            auto& var = current_user_;
            return var;
        }

        bool AuthenticateUser_();

    private:
        User current_user_;
};


#endif // USERS_MANAGER
