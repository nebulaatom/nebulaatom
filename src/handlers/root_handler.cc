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

#include "handlers/root_handler.h"

using namespace CPW::Handlers;

RootHandler::RootHandler(std::string api_version) :
	app_(Application::instance())
	,api_verion_(api_version)
	,route_verification_(true)
{

}

RootHandler::~RootHandler()
{

}

void RootHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	try
	{
		AddRoutes_();

		std::vector<std::string> segments;
		URI(request.getURI()).getPathSegments(segments);

		requested_route_.reset(new CPW::Tools::Route("", segments));

		switch(requested_route_->get_current_route_type())
		{
			case CPW::Tools::RouteType::kEndpoint:
			{
				if(!ManageRequestBody_(request))
				{
					GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "Something was wrong with the Request body.");
					return;
				}

				if(route_verification_)
				{
					if(!IdentifyRoute_())
					{
						GenericResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "The requested endpoint is not available.");
						return;
					}
				}

				if(!InitSecurityProccess_(request, response))
					return;

				break;
			}
			case CPW::Tools::RouteType::kEntrypoint:
			{
				break;
			}
		}

		// Found the corresponding HTTP method
			if(get_actions_strings().find(request.getMethod()) == get_actions_strings().end())
				GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "The client provided a bad HTTP method.");

		// Call the corresponding HTTP method
			get_actions_strings()[request.getMethod()](request, response);
	}
	catch(MySQL::MySQLException& error)
	{
		app_.logger().error("- Error on root_handler.cc on handleRequest(): " + error.displayText());
		GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Error with the database. " + error.displayText());
	}
	catch(JSON::JSONException& error)
	{
		app_.logger().error("- Error on root_handler.cc on handleRequest(): " + error.displayText());
		GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
	}
	catch(std::exception& error)
	{
		app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
		GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
	}
	catch(std::runtime_error& error)
	{
		app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
		GenericResponse_(response, HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
	}
}

bool RootHandler::IdentifyRoute_()
{
	for(auto& it : get_routes_list())
	{
		if(requested_route_->SegmentsToString_() == it->SegmentsToString_())
		{
			requested_route_->set_target(it->get_target());
			return true;
		}
	}

	return false;
}

bool RootHandler::ManageRequestBody_(HTTPServerRequest& request)
{
	std::string request_body = ReadBody_(request.stream());

	if(request_body.empty())
	{
		URI tmp_uri(request.getURI());
		if(!(tmp_uri.getQueryParameters().size() > 0))
			return false;

		if(tmp_uri.getQueryParameters()[0].first != "json")
			return false;

		if(tmp_uri.getQueryParameters()[0].second.empty())
			return false;

		request_body = tmp_uri.getQueryParameters()[0].second;
	}

	if(!Parse_(request_body))
		return false;

	get_current_query_actions()->get_dynamic_json_body() = get_dynamic_json_body();

	return true;
}
