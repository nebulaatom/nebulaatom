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

using namespace CPW;

Route::Route(std::string database_table_target, RouteType current_route_type, std::string target, std::vector<std::string> segments) :
	database_table_target_(database_table_target)
	,current_route_type_(current_route_type)
	,target_(target)
	,segments_(segments)
{
	IdentifyRouteType_();
}

Route::~Route()
{

}


bool Route::IdentifyRouteType_()
{
	 return true;
}
