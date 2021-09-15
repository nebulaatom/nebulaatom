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

using namespace CPW::Factory;

NullHandler::~NullHandler()
{

}

void NullHandler::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	NotFoundError_(request, response);
}

void NullHandler::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	NotFoundError_(request, response);
}

void NullHandler::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	NotFoundError_(request, response);
}

void NullHandler::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	NotFoundError_(request, response);
}

void NullHandler::AddRoutes_()
{

}

void NullHandler::NotFoundError_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
	response.setContentType("application/json");

	Poco::JSON::Object::Ptr object_json = new Poco::JSON::Object;
	object_json->set("Status", "404");
	object_json->set("Message", "Sorry, content not found from" + request.getMethod() + " request.");

	std::ostream& out = response.send();
	object_json->stringify(out);
	out.flush();
}
