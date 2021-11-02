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

HandlerConnection::HandlerConnection(Route route, HandlerFunctor handler) :
	current_route_(route)
	,return_handler_(handler)
{

}

HandlerConnection::~HandlerConnection()
{

}

HandlerFactory::HandlerFactory() :
	api_version_("v0")
{
	CreateConnections_();
}

HandlerFactory::~HandlerFactory()
{
	for(auto it = connections_.begin(); it != connections_.end(); ++it)
		delete it->second;
}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
	auto return_null = [&]()->HTTPRequestHandler*
	{
		return connections_[HandlerType::kNull]->return_handler_();
	};

	try
	{
		URI initial_uri(request.getURI());

		std::vector<std::string> segments;
		initial_uri.getPathSegments(segments);


		std::unique_ptr<Route> requested_route
		(
			new Route
			(
				""
				,segments
			)
		);
		requested_route_ = std::move(requested_route);

		switch(requested_route_->get_current_route_type())
		{
			case RouteType::kEndpoint:
			{
				auto found = FindHandler_(requested_route_->get_segments());
				return connections_[found]->return_handler_();
				break;
			}
			case RouteType::kEntrypoint:
			{
				return connections_[HandlerType::kWeb]->return_handler_();
				break;
			}
		}

	}
	catch (std::exception const& error)
	{
		std::cout << "\nError: " << error.what() << std::endl;
	}

	return return_null();
}

void HandlerFactory::CreateConnections_()
{
	connections_.insert(std::make_pair
	(
		HandlerType::kNull,
		new HandlerConnection
		{
			Route
			(
				"null"
				,std::vector<std::string>{""}
			)
			,[&](){return new CPW::Factory::NullHandler(api_version_);}
		}
	));
	connections_.insert(std::make_pair
	(
		HandlerType::kBusiness,
		new HandlerConnection
		{
			Route
			(
				"business"
				,std::vector<std::string>{"api", api_version_, "business"}
			)
			,[&](){return new CPW::Factory::BusinessHandler(api_version_);}
		}
	));
	connections_.insert(std::make_pair
	(
		HandlerType::kWeb,
		new HandlerConnection
		{
			Route
			(
				"web"
				,std::vector<std::string>{""}
			)
			,[&](){return new CPW::Factory::WebHandler(api_version_);}
		}
	));
}


HandlerType HandlerFactory::FindHandler_(std::vector<std::string> segments)
{
	if(segments.size() < 3)
		return HandlerType::kNull;

	for(auto it : connections_)
	{
		auto target = it.second->current_route_.get_target();
		if(segments[2] == target)
			return it.first;
	}

	return HandlerType::kNull;
}
