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

#include "handlers/frontend_handler.h"

using namespace CPW::Handlers;

FrontendHandler::FrontendHandler(std::string api_version) :
	RootHandler(api_version)
{

}

FrontendHandler::~FrontendHandler()
{

}

void FrontendHandler::AddRoutes_()
{
	get_dynamic_elements()->get_routes_list()->push_back({"",{""}});
}

void FrontendHandler::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	file_manager_.set_operation_type(Tools::OperationType::kDownload);
	if(!file_manager_.CheckFile_(Path(request.getURI())))
	{
		GenericResponse_(response, HTTPResponse::HTTP_NOT_FOUND, "Requested file bad check.");
		return;
	}
	if(!file_manager_.IsSupported_())
	{
		GenericResponse_(response, HTTPResponse::HTTP_BAD_REQUEST, "Requested file is not supported.");
		return;
	}

	file_manager_.ProcessContentType_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType(file_manager_.get_content_type());

	std::ostream& out_reponse = response.send();
	response.setContentLength(file_manager_.get_content_length());

	file_manager_.ManageFile_(out_reponse);

	out_reponse.flush();
}

void FrontendHandler::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
}

void FrontendHandler::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
}

void FrontendHandler::HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
}
