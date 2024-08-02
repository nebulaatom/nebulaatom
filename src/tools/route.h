/*
* Nebula Atom

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

#ifndef NAF_TOOLS_ROUTE
#define NAF_TOOLS_ROUTE


#include <string>
#include <vector>
#include <functional>

#include "Poco/URI.h"


namespace NAF
{
    namespace Tools
    {
        enum class RouteType;
        class Route;
    }
}

using namespace Poco;


enum class NAF::Tools::RouteType
{
    kEntrypoint
    ,kEndpoint
};


class NAF::Tools::Route
{
    public:
        Route();
        Route(std::string route);
        Route(std::vector<std::string> segments);
        ~Route();

        std::string get_route() const { return route_; }
        RouteType get_current_route_type() const {return current_route_type_;}
        std::vector<std::string> get_segments() const {return segments_;}

        void set_current_route_type(RouteType current_route_type) {current_route_type_ = current_route_type;}
        void set_segments(std::vector<std::string> segments) {segments_ = segments;}

        std::string SegmentsToString_();
        bool operator==(const Route& route);

    protected:
        void StringToSegment_(std::string& route);
        void IdentifyRouteType_();

    private:
        std::string route_;
        RouteType current_route_type_;
        std::vector<std::string> segments_;
};

#endif // NAF_TOOLS_ROUTE
