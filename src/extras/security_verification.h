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

#ifndef CPW_EXTRAS_SECURITYVERIFICATION_H
#define CPW_EXTRAS_SECURITYVERIFICATION_H


#include <string>

#include "tools/manage_json.h"
#include "extras/dynamic_elements.h"


namespace CPW
{
    namespace Extras
    {
        enum class SecurityType;
        class SecurityVerification;
    }
}

using namespace Poco;


enum class CPW::Extras::SecurityType
{
    kDisableAll
    ,kEnableAll
};

class CPW::Extras::SecurityVerification
{
    public:
        SecurityVerification();
        ~SecurityVerification();

        std::string get_user() const
        {
            return user_;
        }
        std::string get_password() const
        {
            return password_;
        }
        DynamicElements& get_dynamic_elements()
        {
            auto& var = dynamic_elements_;
            return var;
        }

        void set_user(std::string user)
        {
            user_ = user;
        }
        void set_password(std::string password)
        {
            password_ = password;
        }

        bool AuthenticateUser_();
        bool VerifyPermissions_(Extras::SecurityType security_type, std::string method);
        bool SeePermissionsPerUser_(std::string user, std::string action_type, std::string target);

    protected:
        bool VerifyPermissionGranted_(JSON::Array::Ptr results_array);

    private:
        std::string user_;
        std::string password_;
        DynamicElements dynamic_elements_;
};


#endif // CPW_EXTRAS_SECURITYVERIFICATION_H
