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

#include "extras/security_verification.h"

using namespace CPW::Extras;

SecurityVerification::SecurityVerification()
{

}

SecurityVerification::~SecurityVerification()
{

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
		query_actions->StartDatabase_();
		*query_actions->get_query() << "SELECT * FROM users WHERE username = ? AND password = ?",
			use(table_rows->find("user")->second)
			,use(table_rows->find("password")->second)
			,now
		;
		query_actions->StopDatabase_();

		if(query_actions->get_query()->subTotalRowCount() > 0)
			return true;
		else
			return false;
}

bool SecurityVerification::VerifyPermissions_(std::string method)
{
	// Variables
		auto query_actions = get_current_query_actions();

		std::string user = query_actions->get_table_rows()->at("user");
		std::string action_type = method;
		std::string target = requested_route_->get_target();
		int granted = -1;
		std::size_t rows = 0;

	// Verify permissions for the user
		query_actions->StartDatabase_();

		SeePermissionsPerUser_(user, action_type, target);

		*query_actions->get_query(), into(granted);
		rows = query_actions->get_query()->execute();

		if(rows > 0)
			return granted == 1 ? true : false;

	// Verify permissions for the null user

		query_actions->StartDatabase_();
		SeePermissionsPerUser_("null", action_type, target);

		*query_actions->get_query(), into(granted);
		rows = query_actions->get_query()->execute();

		query_actions->StopDatabase_();

		if(rows > 0)
			return granted == 1 ? true : false;
		else
			return false;

}

void SecurityVerification::SeePermissionsPerUser_(std::string user, std::string action_type, std::string target)
{
	// Variables
		auto query_actions = get_current_query_actions();

	*query_actions->get_query()
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
