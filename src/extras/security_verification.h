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
#include "tools/requests_manager.h"


namespace CPW
{
	namespace Extras
	{
		class SecurityVerification;
	}
}

using namespace Poco;
using namespace Poco::Util;


class CPW::Extras::SecurityVerification:
	public CPW::Tools::RequestsManager
	,public CPW::Tools::ManageJSON
{
	public:
		SecurityVerification();
		~SecurityVerification();

		bool AuthenticateUser_();
		bool VerifyPermissions_(std::string method);
		void SeePermissionsPerUser_(std::string user, std::string action_type, std::string target);
};


#endif // CPW_EXTRAS_SECURITYVERIFICATION_H
