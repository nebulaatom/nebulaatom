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

RootHandler::RootHandler(std::string api_version) :
	api_verion_(api_version)
{
	current_query_actions_ = new QueryActions();
	routes_list_ = new std::list<Route*>;
}

RootHandler::~RootHandler()
{
	delete current_query_actions_;
	for(auto it : *routes_list_)
		delete it;
	delete routes_list_;
}

void RootHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
	try
	{
		AddRoutes_();
		ReadJSONBody_(request);

		if(!IdentifyRoute_(request))
		{
			BasicError_(response, "The requested endpoint is not available.", HTTPResponse::HTTP_NOT_FOUND);
			return;
		}

		if(!SecurityVerification_(request, response))
			return;

		if(request.getMethod() == "GET")
			HandleGETMethod_(request, response);
		else if(request.getMethod() == "POST")
			HandlePOSTMethod_(request, response);
		else if(request.getMethod() == "PUT")
			HandlePUTMethod_(request, response);
		else if(request.getMethod() == "DEL")
			HandleDELMethod_(request, response);
		else
			BasicError_(response, "The client provided a bad HTTP method.", HTTPResponse::HTTP_BAD_REQUEST);
	}
	catch(std::exception error)
	{
		std::cout << "\nError on root_handler.cc: " << error.what();
	}
}

bool RootHandler::SecurityVerification_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	if(AuthenticateUser_())
	{
		if(!VerifyPermissions_(request))
		{
			BasicError_(response, "The user does not have the permissions to perform this operation.", HTTPResponse::HTTP_UNAUTHORIZED);
			return false;
		}
		return true;
	}
	else
	{
		BasicError_(response, "Unauthorized user or wrong user or password.", HTTPResponse::HTTP_UNAUTHORIZED);
		return false;
	}
}

bool RootHandler::AuthenticateUser_()
{
	// Prepare the row
		auto table_rows = current_query_actions_->get_table_rows();
		table_rows->insert(std::make_pair("user", ""));
		table_rows->insert(std::make_pair("password", ""));

	// Verify the key-values
		if(dynamic_json_body_["auth"].isEmpty() || dynamic_json_body_["auth"]["user"].isEmpty() || dynamic_json_body_["auth"]["password"].isEmpty())
		{
			table_rows->find("user")->second = "null";
			table_rows->find("password")->second = "null";
		}
		else
		{
			table_rows->find("user")->second = dynamic_json_body_["auth"]["user"].toString();
			table_rows->find("password")->second = dynamic_json_body_["auth"]["password"].toString();
		}

	// Execute the query
		current_query_actions_->ResetQuery_();
		current_query_actions_->get_query() << "SELECT * FROM users WHERE username = ? AND password = ?",
			use(table_rows->find("user")->second),
			use(table_rows->find("password")->second)
		;
		current_query_actions_->get_query().execute();
		if(current_query_actions_->get_query().subTotalRowCount() > 0)
			return true;
		else
			return false;
}

bool RootHandler::VerifyPermissions_(HTTPServerRequest& request)
{
	Util::Application& app = Util::Application::instance();

	std::string user = get_current_query_actions()->get_table_rows()->at("user");
	std::string action_type = request.getMethod();
	std::string target = requested_route_->get_target();

	app.logger().information("\ntarget: " + target + ", user: " + user + ", action type: " + action_type);

	current_query_actions_->ResetQuery_();
	int result;
	current_query_actions_->get_query()
		<<
			"SELECT COUNT(1) "
			"FROM permissions_log pl, permissions p, users u "
			"WHERE "
			"	u.username = ? "
			"	AND pl.type = ? "
			"	AND p.name = ? "
			"	AND pl.id_permission = p.id "
			"	AND pl.id_user = u.id"
			";"
		,
		use(user),
		use(action_type),
		use(target),
		into(result)
	;

	// Execute the query
		current_query_actions_->get_query().execute();

		if(result > 0)
			return true;
		else
			return false;
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
			,std::vector<std::string>{""}
		)
	);
	requested_route_ = std::move(requested_route);

	for(auto it : *routes_list_)
	{
		auto sub_segments = it->get_segments();
		auto found = std::search(segments.begin(), segments.end(), sub_segments.begin(), sub_segments.end());

		if(found != segments.end())
		{
			requested_route_->set_target(it->get_target());
			return true;
		}
	}

	return false;
}

void RootHandler::ReadJSONBody_(HTTPServerRequest& request)
{
	// Read the JSON
		std::string json_body;
		StreamCopier::copyToString(request.stream(), json_body);
		if(json_body.empty())
			return;

	// Parse JSON
		JSON::Parser parser;
		JSON::Object::Ptr object_json = parser.parse(json_body).extract<JSON::Object::Ptr>();
		dynamic_json_body_ = *object_json;

}
