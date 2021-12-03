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

#include "common_responses.h"

using namespace CPW;

CommonResponses::CommonResponses()
{
	FillResponses_();
}

CommonResponses::~CommonResponses()
{

}

void CommonResponses::GenericResponse_(HTTPServerResponse& response, HTTPResponse::HTTPStatus status, std::string message)
{
	response.setStatus(status);
	response.setContentType("application/json");

	JSON::Object::Ptr object_json = new JSON::Object;

	auto found = responses_.find(status);
	if(found != responses_.end())
	{
		object_json->set("Status", responses_[status].second);
		object_json->set("Message", message);
	}
	else
	{
		object_json->set("Status", responses_[HTTPResponse::HTTP_INTERNAL_SERVER_ERROR].second);
		object_json->set("Message", "Error on HTTPStatus");
	}

	std::ostream& out = response.send();
	object_json->stringify(out);
	out.flush();
}

void CommonResponses::FillResponses_()
{
	responses_.emplace(std::make_pair
	(
		HTTPResponse::HTTP_BAD_REQUEST
		,std::make_pair(ResponseType::kWarning, "Client-side input fails validation.")
	));
	responses_.emplace(std::make_pair
	(
		HTTPResponse::HTTP_UNAUTHORIZED
		,std::make_pair(ResponseType::kWarning, "The user isn’t not authorized to access to this resource.")
	));
	responses_.emplace(std::make_pair
	(
		HTTPResponse::HTTP_FORBIDDEN
		,std::make_pair(ResponseType::kWarning, "The user is authenticated, but it’s not allowed to access to this resource.")
	));
	responses_.emplace(std::make_pair
	(
		HTTPResponse::HTTP_NOT_FOUND
		,std::make_pair(ResponseType::kWarning, "Resource is not found.")
	));
	responses_.emplace(std::make_pair
	(
		HTTPResponse::HTTP_INTERNAL_SERVER_ERROR
		,std::make_pair(ResponseType::kError, "Something was wrong.")
	));
	responses_.emplace(std::make_pair
	(
		HTTPResponse::HTTP_BAD_GATEWAY
		,std::make_pair(ResponseType::kError, "Invalid response from an upstream server.")
	));
	responses_.emplace(std::make_pair
	(
		HTTPResponse::HTTP_SERVICE_UNAVAILABLE
		,std::make_pair(ResponseType::kError, "Something unexpected happened on server side.")
	));
}
