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

#ifndef ATOM_SECURITY_SECURITY_VERIFICATION
#define ATOM_SECURITY_SECURITY_VERIFICATION


#include <string>
#include <list>
#include <stdexcept>

#include <Poco/Tuple.h>

#include "security/users_manager.h"
#include "security/permissions_manager.h"
#include "tools/output_logger.h"


namespace Atom
{
    namespace Extras
    {
        enum class SecurityType;
        class SecurityVerification;
    }
}

using namespace Poco;


enum class Atom::Extras::SecurityType
{
    kDisableAll
    ,kEnableAll
};

class Atom::Extras::SecurityVerification
{
    public:
        SecurityVerification();

        Security::PermissionsManager& get_permissions_manager()
        {
            auto& var = permissions_manager_;
            return var;
        }
        Security::UsersManager& get_users_manager()
        {
            auto& var = users_manager_;
            return var;
        }
        Extras::SecurityType get_security_type() const { return security_type_; }
        
        void set_security_type(Extras::SecurityType security_type) { security_type_ = security_type; }

        bool VerifyRoutesPermissions_(Tools::Route& route, std::string action_type);

    private:
        Security::PermissionsManager permissions_manager_;
        Security::UsersManager users_manager_;
        Extras::SecurityType security_type_;
};


#endif // ATOM_SECURITY_SECURITY_VERIFICATION
