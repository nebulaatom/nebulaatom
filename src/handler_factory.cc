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

HandlerFactory::HandlerFactory() :
	api_version_("v0")
{
	PrepareEndpoints_();
}

HandlerFactory::~HandlerFactory()
{

}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
	auto return_null = [&]()->HTTPRequestHandler*
	{
		return endpoints_handlers_[Endpoint::kNull].return_handler();
	};

	try
	{
		URI initial_uri(request.getURI());

		std::vector<std::string> segments;
		initial_uri.getPathSegments(segments);

	if(segments.size() > 0)
			return endpoints_handlers_[GetEndpoint_(segments)].return_handler();
		else
			return return_null();

		/*if(segments.size() < 1 || segments.size() <= 2)
			return endpoints_handlers_[Endpoint::kWeb]();
		else
		{
			if(segments[0] == "api" && segments[1] == api_version_)
			{
				auto end_type = endpoints_keys_.find(segments[2]);
				if(end_type != endpoints_keys_.end())
					return endpoints_handlers_[end_type->second]();
				else
					return return_null();
			}
			else if(segments[0] == "api" && segments[1] != api_version_)
				return return_null();
			else
				return endpoints_handlers_[Endpoint::kWeb]();
		}*/
	}
	catch (std::exception const& error)
	{
		std::cout << "\nError: " << error.what() << std::endl;
	}

	return return_null();
}

void HandlerFactory::PrepareEndpoints_()
{
	endpoints_keys_.insert(std::make_pair("/api/"+api_version_+"/business", Endpoint::kBusiness));
	endpoints_handlers_.insert
	(
		std::make_pair
		(
			Endpoint::kBusiness,
			[](){return new CPW::Factory::BusinessHandler();}
		)
	);
	endpoints_keys_.insert(std::make_pair("null", Endpoint::kNull));
	endpoints_handlers_.insert
	(
		std::make_pair
		(
			Endpoint::kNull,
			[](){return new CPW::Factory::NullHandler();}
		)
	);
}


HandlerFactory::Endpoint HandlerFactory::GetEndpoint_(std::vector<std::string> segments)
{
	for(auto it : endpoints_handlers_)
	{
		auto sub_segments = it.second.segments;
		auto found = std::search(segments.begin(), segments.end(), sub_segments.begin(), sub_segments.end());

		if(found != segments.end())
			return it.first;
	}

	return Endpoint::kNull;
}
