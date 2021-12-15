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
		auto query_actions = dynamic_elements_.get_query_actions();
		auto result_json = query_actions->get_result_json();
		auto& json_auth = query_actions->get_dynamic_json_body()["pair-information"][0]["auth"];
		auto& iquals = query_actions->get_current_filters_()->get_iquals_conditions();

	// Verify the key-values
		if(json_auth["user"].isEmpty() || json_auth["password"].isEmpty())
		{
			iquals.emplace(std::make_pair("username", "null"));
			iquals.emplace(std::make_pair("password", "null"));
		}
		else
		{
			iquals.emplace(std::make_pair("username", json_auth["user"].toString()));
			iquals.emplace(std::make_pair("password", json_auth["password"].toString()));
		}

	// Execute the query
		query_actions->ComposeQuery_(Core::TypeAction::kSelect, "users");
		query_actions->ExecuteQuery_();

		auto object_tmp = query_actions->ExtractArray_(result_json->get("results"));

		if(object_tmp->size() > 0)
			return true;
		else
			return false;
}

bool SecurityVerification::VerifyPermissions_(std::string method)
{
	// Variables
		auto query_actions = dynamic_elements_.get_query_actions();
		auto result_json = query_actions->get_result_json();
		std::string target = dynamic_elements_.get_requested_route()->get_target();
		auto& iquals = query_actions->get_current_filters_()->get_iquals_conditions();
		int granted = -1;

	// Find user
		std::string user;
		auto found = iquals.find("username");
		if(found != iquals.end())
			user = found->second.get_value();
		else
			user = "null";

	// Verify permissions for the users
		for(auto it : std::vector{user, std::string("null")})
		{
			SeePermissionsPerUser_(it, method, target);

			auto array_tmp = query_actions->ExtractArray_(result_json->get("results"));
			if(array_tmp->size() < 1)
				continue;

			auto object_tmp = query_actions->ExtractObject_(array_tmp->get(0));
			if(object_tmp->get("granted").isEmpty())
				continue;
			if(!object_tmp->get("granted").isInteger())
				continue;

			granted = std::stoi(object_tmp->get("granted").toString());

			return granted == 1 ? true : false;
		}

		return false;
}

void SecurityVerification::SeePermissionsPerUser_(std::string user, std::string action_type, std::string target)
{
	// Variables
		auto query_actions = dynamic_elements_.get_query_actions();
		auto& iquals = query_actions->get_current_filters_()->get_iquals_conditions();
		auto& fields = query_actions->get_current_filters_()->get_fields();

	// Clear previous values
		iquals.clear();
		fields.clear();

	// Filters
		fields.push_back({"pl.granted", false});
		iquals.emplace(std::make_pair("u.username", Tools::ValuesProperties{user, true}));
		iquals.emplace(std::make_pair("pl.type", Tools::ValuesProperties{action_type, true}));
		iquals.emplace(std::make_pair("p.name", Tools::ValuesProperties{target, true}));
		iquals.emplace(std::make_pair("pl.id_permission", Tools::ValuesProperties{"p.id", false}));
		iquals.emplace(std::make_pair("pl.id_user", Tools::ValuesProperties{"u.id", false}));

	// Data sentences
		query_actions->ComposeQuery_(Core::TypeAction::kSelect, "permissions_log pl, permissions p, users u");
		query_actions->ExecuteQuery_();
}
