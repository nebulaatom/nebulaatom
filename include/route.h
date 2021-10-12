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

#ifndef CPW_ROUTE_H
#define CPW_ROUTE_H


#include <string>
#include <vector>
#include <functional>


namespace CPW
{
	enum class RouteType;
	class Route;
}

enum class CPW::RouteType
{
	kEntrypoint
	,kEndpoint
};


class CPW::Route
{
	public:
		Route(std::string target, std::vector<std::string> segments);
		~Route();

		RouteType get_current_route_type() const {return current_route_type_;}
		std::string get_target() const {return target_;}
		std::vector<std::string> get_segments() const {return segments_;}

		void set_current_route_type(RouteType current_route_type) {current_route_type_ = current_route_type;}
		void set_target(std::string target) {target_ = target;}
		void set_segments(std::vector<std::string> segments) {segments_ = segments;}

		std::string SegmentsToString_();

	protected:
		void IdentifyRouteType_();

	private:
		RouteType current_route_type_;
		std::string target_;
		std::vector<std::string> segments_;
};

#endif // CPW_ROUTE_H
