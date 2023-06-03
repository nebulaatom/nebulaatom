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


#ifndef USERS_MANAGER_H
#define USERS_MANAGER_H


#include <string>

#include "query/query_actions.h"
#include "security/user.h"


namespace CPW
{
    namespace Security
    {
        class UsersManager;
    }
}


class CPW::Security::UsersManager
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
        Query::QueryActions query_manager_;
};


#endif // USERS_MANAGER_H
