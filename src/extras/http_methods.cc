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

#include "extras/http_methods.h"

using namespace CPW::Extras;

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
