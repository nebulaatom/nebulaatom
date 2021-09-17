/*
 * CPW Woodpecker Server
 * Copyright (C) 2021 CPW Online support@cpwonline.org
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

#ifndef CPW_ERRORREPORT_H
#define CPW_ERRORREPORT_H


#include <string>

#include <Poco/Net/HTTPServerResponse.h>


namespace CPW
{
	class ErrorReport;
}

using namespace Poco;
using namespace Poco::Net;


class CPW::ErrorReport
{
	public:
		ErrorReport();
		~ErrorReport();
		void BasicError_(HTTPServerResponse& response, std::string message, HTTPResponse::HTTPStatus status);
};

#endif // CPW_ERRORREPORT_H
