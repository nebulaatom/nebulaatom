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

#include "route.h"

using namespace AtomTools;

Route::Route() :
    current_route_type_(RouteType::kEntrypoint)
    ,segments_({})
{

}
Route::Route(std::string route) :
    current_route_type_(RouteType::kEntrypoint)
{
    StringToSegment(route);
}

Route::Route(std::vector<std::string> segments) :
    current_route_type_(RouteType::kEntrypoint)
    ,segments_(segments)
{
    IdentifyRouteType_();
}

Route::~Route()
{

}

void Route::StringToSegment(std::string route)
{
    URI(route).getPathSegments(segments_);
}

void Route::IdentifyRouteType_()
{
    if(segments_.size() > 0)
    {
        if(segments_.front() == "api")
            current_route_type_ = RouteType::kEndpoint;
        else
            current_route_type_ = RouteType::kEntrypoint;
    }
    else
        current_route_type_ = RouteType::kEntrypoint;
}

std::string Route::SegmentsToString_()
{
    std::string segments_to_string = "";
    for(auto it : segments_)
    {
        segments_to_string += "/" + it;
    }
    return segments_to_string;
}
