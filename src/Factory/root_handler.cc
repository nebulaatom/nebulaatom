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

#include "Factory/root_handler.h"

using namespace CPW::Factory;


HTTPMethods::HTTPMethods()
{

}

HTTPMethods::~HTTPMethods()
{

}


void HTTPMethods::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	get_current_query_actions()->IdentifyFilters_();
	get_current_query_actions()->ComposeQuery_(TypeAction::kSelect, requested_route_->get_target());
	get_current_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	get_current_query_actions()->get_result_json()->stringify(out);
	out.flush();
}

void HTTPMethods::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	get_current_query_actions()->IdentifyFilters_();
	get_current_query_actions()->ComposeQuery_(TypeAction::kInsert, requested_route_->get_target());
	get_current_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	out << "{POST}";
	out.flush();
}

void HTTPMethods::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	get_current_query_actions()->IdentifyFilters_();
	get_current_query_actions()->ComposeQuery_(TypeAction::kUpdate, requested_route_->get_target());
	get_current_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	out << "{PUT}";
	out.flush();
}

void HTTPMethods::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	get_current_query_actions()->IdentifyFilters_();
	get_current_query_actions()->ComposeQuery_(TypeAction::kDelete, requested_route_->get_target());
	get_current_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	out << "{DEL}";
	out.flush();
}

DynamicElements::DynamicElements() :
	app_(Application::instance())
{
	current_query_actions_ = new QueryActions();
}

DynamicElements::~DynamicElements()
{
	delete current_query_actions_;
	for(auto it : routes_list_)
		delete it;
}


SecurityVerification::SecurityVerification()
{

}

SecurityVerification::~SecurityVerification()
{

}

bool SecurityVerification::InitSecurityProccess_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	if(AuthenticateUser_())
	{
		if(!VerifyPermissions_(request))
		{
			GenericResponse_(response, HTTPResponse::HTTP_UNAUTHORIZED, "The user does not have the permissions to perform this operation.");
			return false;
		}
		return true;
	}
	else
	{
		GenericResponse_(response, HTTPResponse::HTTP_UNAUTHORIZED, "Unauthorized user or wrong user or password.");
		return false;
	}
}

bool SecurityVerification::AuthenticateUser_()
{
	// Variables
		auto json_body = get_dynamic_json_body();
		auto query_actions = get_current_query_actions();
		auto table_rows = query_actions->get_table_rows();

	// Prepare the row
		table_rows->insert(std::make_pair("user", ""));
		table_rows->insert(std::make_pair("password", ""));

	// Verify the key-values
		if
		(
			json_body["pair-information"].isEmpty()
			|| json_body["pair-information"][0]["auth"]["user"].isEmpty()
			|| json_body["pair-information"][0]["auth"]["password"].isEmpty()
		)
		{
			table_rows->find("user")->second = "null";
			table_rows->find("password")->second = "null";
		}
		else
		{
			table_rows->find("user")->second = json_body["pair-information"][0]["auth"]["user"].toString();
			table_rows->find("password")->second = json_body["pair-information"][0]["auth"]["password"].toString();
		}

	// Execute the query
		query_actions->ResetQuery_();
		query_actions->get_query() << "SELECT * FROM users WHERE username = ? AND password = ?",
			use(table_rows->find("user")->second),
			use(table_rows->find("password")->second)
		;
		query_actions->get_query().execute();

		if(query_actions->get_query().subTotalRowCount() > 0)
			return true;
		else
			return false;
}

bool SecurityVerification::VerifyPermissions_(HTTPServerRequest& request)
{
	// Variables
		auto query_actions = get_current_query_actions();

		std::string user = query_actions->get_table_rows()->at("user");
		std::string action_type = request.getMethod();
		std::string target = requested_route_->get_target();
		int granted = -1;
		std::size_t rows = 0;

	get_app().logger().information("Target: " + target + ", user: " + user + ", action type: " + action_type);

	// Verify permissions for the user
		SeePermissionsPerUser_(user, action_type, target);
		query_actions->get_query(), into(granted);
		rows = query_actions->get_query().execute();

		if(rows > 0)
			return granted == 1 ? true : false;

	// Verify permissions for the null user

		SeePermissionsPerUser_("null", action_type, target);
		query_actions->get_query(), into(granted);
		rows = query_actions->get_query().execute();

		if(rows > 0)
			return granted == 1 ? true : false;
		else
			return false;

}

void SecurityVerification::SeePermissionsPerUser_(std::string user, std::string action_type, std::string target)
{
	// Variables
		auto query_actions = get_current_query_actions();

	query_actions->ResetQuery_();
	query_actions->get_query()
		<<
			"SELECT "
			"	pl.granted "
			"FROM permissions_log pl, permissions p, users u "
			"WHERE "
			"	u.username = ? "
			"	AND pl.type = ? "
			"	AND p.name = ? "
			"	AND pl.id_permission = p.id "
			"	AND pl.id_user = u.id"
			";"
		,use(user)
		,use(action_type)
		,use(target)
	;
}

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

		URI initial_uri(request.getURI());
		std::vector<std::string> segments;

		initial_uri.getPathSegments(segments);

		std::unique_ptr<Route> requested_route(new Route("", segments));
		requested_route_ = std::move(requested_route);

		switch(requested_route_->get_current_route_type())
		{
			case RouteType::kEndpoint:
			{
				if(!Parse_(ReadBody_(request.stream())))
				{
					GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "Something was wrong with the JSON data.");
					return;
				}

				get_current_query_actions()->get_dynamic_json_body() = get_dynamic_json_body();

				if(route_verification_)
				{
					if(!IdentifyRoute_(request))
					{
						GenericResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "The requested endpoint is not available.");
						return;
					}
				}

				if(!InitSecurityProccess_(request, response))
					return;

				break;
			}
			case RouteType::kEntrypoint:
			{
				break;
			}
		}

		if(request.getMethod() == "GET")
			HandleGETMethod_(request, response);
		else if(request.getMethod() == "POST")
			HandlePOSTMethod_(request, response);
		else if(request.getMethod() == "PUT")
			HandlePUTMethod_(request, response);
		else if(request.getMethod() == "DEL")
			HandleDELMethod_(request, response);
		else
			GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
	}
	catch(std::exception error)
	{
		app_.logger().error("- Error on root_handler.cc on handleRequest(): " + std::string(error.what()));
	}
}

bool RootHandler::IdentifyRoute_(HTTPServerRequest& request)
{
	URI uri(request.getURI());
	std::vector<std::string> segments;
	uri.getPathSegments(segments);

	std::unique_ptr<Route> requested_route
	(
		new Route
		(
			""
			,segments
		)
	);
	requested_route_ = std::move(requested_route);

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
