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
	dynamic_elements_->get_query_actions()->IdentifyFilters_();
	dynamic_elements_->get_query_actions()->ComposeQuery_(Core::TypeAction::kSelect, dynamic_elements_->get_requested_route()->get_target());
	dynamic_elements_->get_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	dynamic_elements_->get_query_actions()->get_result_json()->stringify(out);
	out.flush();
}

void HTTPMethods::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	dynamic_elements_->get_query_actions()->IdentifyFilters_();
	dynamic_elements_->get_query_actions()->ComposeQuery_(Core::TypeAction::kInsert, dynamic_elements_->get_requested_route()->get_target());
	dynamic_elements_->get_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	out << "{POST}";
	out.flush();
}

void HTTPMethods::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	dynamic_elements_->get_query_actions()->IdentifyFilters_();
	dynamic_elements_->get_query_actions()->ComposeQuery_(Core::TypeAction::kUpdate, dynamic_elements_->get_requested_route()->get_target());
	dynamic_elements_->get_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	out << "{PUT}";
	out.flush();
}

void HTTPMethods::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	dynamic_elements_->get_query_actions()->IdentifyFilters_();
	dynamic_elements_->get_query_actions()->ComposeQuery_(Core::TypeAction::kDelete, dynamic_elements_->get_requested_route()->get_target());
	dynamic_elements_->get_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	out << "{DEL}";
	out.flush();
}
