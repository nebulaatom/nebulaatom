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
	api_verion_(api_version),
	current_route_("")
{
	current_query_actions_ = new QueryActions();
	routes_list_ = new std::set<std::string>;
}

RootHandler::~RootHandler()
{
	delete current_query_actions_;
	delete routes_list_;
}

void RootHandler::SecurityVerification_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	if(AuthenticateUser_(request))
	{
		auto table_rows = get_current_query_actions()->get_table_rows_();
		URI uri(request.getURI());
		std::vector<std::string> segments;
		uri.getPathSegments(segments);

		if(!VerifyPermissions_(table_rows->find("user")->second, segments.back(), request.getMethod()))
		{
			ErrorReport_(response, "The user does not have the permissions to perform this operation.", HTTPResponse::HTTP_UNAUTHORIZED);
			return;
		}

	}
	else
	{
		ErrorReport_(response, "Unauthorized user or wrong user or password.", HTTPResponse::HTTP_UNAUTHORIZED);
		return;
	}
}

bool RootHandler::AuthenticateUser_(HTTPServerRequest& request)
{
	ReadJSON_(request);

	// Verify the key-values
		if(dynamic_json_body_["auth"].isEmpty() || dynamic_json_body_["auth"]["user"].isEmpty() || dynamic_json_body_["auth"]["password"].isEmpty())
			return false;

	// Prepare the row
		auto table_rows = current_query_actions_->get_table_rows_();
		table_rows->insert(std::make_pair("user", ""));
		table_rows->insert(std::make_pair("password", ""));

		table_rows->find("user")->second = dynamic_json_body_["auth"]["user"].toString();
		table_rows->find("password")->second = dynamic_json_body_["auth"]["password"].toString();

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

bool RootHandler::VerifyPermissions_(std::string user, std::string action, std::string action_type)
{
	current_query_actions_->ResetQuery_();
	int result;
	current_query_actions_->get_query()
		<<
			"SELECT COUNT(1)"
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
		use(action),
		into(result)
	;

	// Execute the query
		current_query_actions_->get_query().execute();
		std::cout << "\nResult: " << result;

		if(result > 0)
			return true;
		else
			return false;
}
void RootHandler::ErrorReport_(HTTPServerResponse& response, std::string message, HTTPResponse::HTTPStatus status)
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
void RootHandler::ReadJSON_(HTTPServerRequest& request)
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
