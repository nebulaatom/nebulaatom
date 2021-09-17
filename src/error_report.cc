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

#include "error_report.h"

using namespace CPW;

ErrorReport::ErrorReport()
{

}

ErrorReport::~ErrorReport()
{

}

void ErrorReport::BasicError_(HTTPServerResponse& response, std::string message, HTTPResponse::HTTPStatus status)
{
	response.setStatus(status);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	out
		<< "{"
			<< "\"status\":\"" << size_t(status) << "\","
			<< "\"message\":\"" << message << "\""
		<< "}"
	;
	out.flush();
}
