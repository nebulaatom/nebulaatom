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

#include "handlers/null_handler.h"

using namespace CPW::Handlers;

NullHandler::~NullHandler()
{

}

void NullHandler::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
    HTMLResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Sorry, content not found from" + request.getMethod() + " request.");
}

void NullHandler::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
    HTMLResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Sorry, content not found from" + request.getMethod() + " request.");
}

void NullHandler::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
    HTMLResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Sorry, content not found from" + request.getMethod() + " request.");
}

void NullHandler::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
    HTMLResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Sorry, content not found from" + request.getMethod() + " request.");
}

void NullHandler::AddRoutes_()
{

}
