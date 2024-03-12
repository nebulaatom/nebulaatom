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

#include "security/security_verification.h"

using namespace AtomExtras;

SecurityVerification::SecurityVerification()
{

}

bool SecurityVerification::VerifyRoutesPermissions_(Tools::Route& route, std::string action_type)
{
    try
    {
        auto p = permissions_manager_.FindPermission_(route, users_manager_.get_current_user().get_username(), action_type);

        // Permission not found
        switch(security_type_)
        {
            case SecurityType::kDisableAll:
            {
                if(p == permissions_manager_.get_permissions().end())
                    return false;

                break;
            }
            case SecurityType::kEnableAll:
            {
                if(p == permissions_manager_.get_permissions().end())
                    return true;
                
                break;
            }
        }
    }
    catch(const std::exception& error)
    {
        std::cerr << "- SecurityVerification::VerifyRoutesPermissions_(): " << std::string(error.what());
        return false;
    }
    return true;
}
