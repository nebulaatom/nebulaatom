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

#include "handler_factory.h"

using namespace CPW;

HandlerFactory::HandlerFactory()
{
	PrepareEndpoints_();
}

HandlerFactory::~HandlerFactory()
{

}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
	URI initial_uri(request.getURI());
	auto end_type = endpoints_keys_.find(initial_uri.getPath());
	if(end_type != endpoints_keys_.end())
		return endpoints_handlers_[end_type->second]();
	else
		return endpoints_handlers_[Endpoint::kNull]();
}

}
