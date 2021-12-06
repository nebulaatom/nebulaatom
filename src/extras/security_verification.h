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

#include "Poco/Util/ServerApplication.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "tools/common_responses.h"
#include "tools/manage_json.h"
#include "extras/http_methods.h"


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
	public CPW::Extras::HTTPMethods
	,public CPW::Tools::CommonResponses
	,public CPW::Tools::ManageJSON
{
	public:
		SecurityVerification();
		~SecurityVerification();

		bool InitSecurityProccess_(HTTPServerRequest& request, HTTPServerResponse& response);
		bool AuthenticateUser_();
		bool VerifyPermissions_(HTTPServerRequest& request);
		void SeePermissionsPerUser_(std::string user, std::string action_type, std::string target);
};


#endif // CPW_EXTRAS_SECURITYVERIFICATION_H
