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

#include "http/http_methods.h"

using namespace CPW::HTTP;

HTTPMethods::HTTPMethods()
{

}

HTTPMethods::~HTTPMethods()
{

}

void HTTPMethods::HandleGETMethod_()
{
    if(!QueryProcess_(Query::TypeAction::kSelect))
        return;

    dynamic_elements_->get_response()->setStatus(HTTPResponse::HTTP_OK);
    dynamic_elements_->get_response()->setContentType("application/json");
    dynamic_elements_->get_response()->setChunkedTransferEncoding(true);

    std::ostream& out = dynamic_elements_->get_response()->send();
    dynamic_elements_->get_query_actions()->get_result_json()->stringify(out);
    out.flush();
}

void HTTPMethods::HandlePOSTMethod_()
{
    if(!QueryProcess_(Query::TypeAction::kInsert))
        return;

    responses_.GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_OK, "Ok.");
}

void HTTPMethods::HandlePUTMethod_()
{
    if(!QueryProcess_(Query::TypeAction::kUpdate))
        return;

    responses_.GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_OK, "Ok.");
}

void HTTPMethods::HandleDELMethod_()
{
    if(!QueryProcess_(Query::TypeAction::kDelete))
        return;

    responses_.GenericResponse_(*dynamic_elements_->get_response(), HTTPResponse::HTTP_OK, "Ok.");
}

bool HTTPMethods::QueryProcess_(Query::TypeAction action)
{
    dynamic_elements_->get_query_actions()->IdentifyFilters_();
    dynamic_elements_->get_query_actions()->ComposeQuery_(action, dynamic_elements_->get_requested_route()->get_target());
    return dynamic_elements_->get_query_actions()->ExecuteQuery_(*dynamic_elements_->get_response());
}
