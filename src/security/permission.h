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


#ifndef PERMISSION_H
#define PERMISSION_H


#include <string>
#include <memory>

#include "tools/route.h"
#include "security/user.h"


namespace Atom
{
    namespace Security
    {
        enum class ActionType;
        class Permission;
    }
}


enum class AtomSecurity::ActionType
{
    kCreate
    ,kRead
    ,kUpdate
    ,kDelete
};

class AtomSecurity::Permission
{
    public:
        Permission();
        Permission(Tools::Route route_, std::shared_ptr<User> user_, ActionType action_type);

        Tools::Route& get_route()
        {
            auto& var = route_;
            return var;
        }
        std::shared_ptr<User>& get_user()
        {
            auto& var = user_;
            return var;
        }
        ActionType get_action_type() const { return action_type_; }

        void set_route(Tools::Route route) { route_ = route; }
        void set_action_type(ActionType& action_type) { action_type_ = action_type; }

    private:
        Tools::Route route_;
        std::shared_ptr<User> user_;
        ActionType action_type_;
};


#endif // PERMISSION_H
