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

#ifndef CPW_EXTRAS_DYNAMICELEMENTS_H
#define CPW_EXTRAS_DYNAMICELEMENTS_H


#include <list>
#include <vector>
#include <memory>

#include "Poco/Util/ServerApplication.h"
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "query/query_actions.h"
#include "tools/route.h"


namespace CPW
{
    namespace Extras
    {
        class DynamicElements;
    }
}

using namespace Poco;
using namespace Poco::Util;


class CPW::Extras::DynamicElements
{
    public:
        using RouteSharedPtr = std::shared_ptr<CPW::Tools::Route>;

        DynamicElements();
        DynamicElements(DynamicElements& dynamic_elements);
        ~DynamicElements();

        std::list<Tools::Route>& get_routes_list()
        {
            auto& var = routes_list_;
            return var;
        }
        std::shared_ptr<Query::QueryActions>& get_query_actions()
        {
            auto& var = query_actions_;
            return var;
        }
        Application& get_app() const {return app_;};
        RouteSharedPtr get_requested_route() const {return requested_route_;}
        HTTPServerRequest* get_request() const {return request_;}
        HTTPServerResponse* get_response() const {return response_;}

        void set_requested_route(RouteSharedPtr requested_route) {requested_route_ = requested_route;}
        void set_request(HTTPServerRequest& request) {request_ = &request;}
        void set_response(HTTPServerResponse& response) {response_ = &response;}

    protected:
        RouteSharedPtr requested_route_;

    private:
        std::list<Tools::Route> routes_list_;
        std::shared_ptr<Query::QueryActions> query_actions_;
        Application& app_;
        HTTPServerRequest* request_;
        HTTPServerResponse* response_;
};

#endif // CPW_EXTRAS_DYNAMICELEMENTS_H
