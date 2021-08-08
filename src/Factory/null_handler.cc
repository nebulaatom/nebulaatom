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

#include "Factory/null_handler.h"

namespace CPW
{
namespace Factory
{


NullHandler::NullHandler()
{

}

NullHandler::~NullHandler()
{

}

void NullHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	if(request.getMethod() == "GET")
		HandleGETMethod_(request, response);
	else if(request.getMethod() == "POST")
		HandlePOSTMethod_(request, response);
	else if(request.getMethod() == "PUT")
		HandlePUTMethod_(request, response);
	else if(request.getMethod() == "DEL")
		HandleDELMethod_(request, response);
}

void NullHandler::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
	response.setContentType("text/html");

	std::ostream& out = response.send();
	out
		<< "<h1>Sorry, content not found from " << request.getMethod() << " request</h1>"
		<< "<p>CPW Woodpecker</p>"
	;
	out.flush();
}

void NullHandler::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
	response.setContentType("text/html");

	std::ostream& out = response.send();
	out
		<< "<h1>Sorry, content not found from " << request.getMethod() << " request</h1>"
		<< "<p>CPW Woodpecker</p>"
	;
	out.flush();
}

void NullHandler::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
	response.setContentType("text/html");

	std::ostream& out = response.send();
	out
		<< "<h1>Sorry, content not found from " << request.getMethod() << " request</h1>"
		<< "<p>CPW Woodpecker</p>"
	;
	out.flush();
}

void NullHandler::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
	response.setContentType("text/html");

	std::ostream& out = response.send();
	out
		<< "<h1>Sorry, content not found from " << request.getMethod() << " request</h1>"
		<< "<p>CPW Woodpecker</p>"
	;
	out.flush();
}

}
}
