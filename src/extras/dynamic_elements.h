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

#include "Poco/Util/ServerApplication.h"

#include "core/query_actions.h"
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
		DynamicElements();
		~DynamicElements();

		std::list<CPW::Tools::Route*>& get_routes_list()
		{
			std::list<CPW::Tools::Route*>& var = routes_list_;
			return var;
		}
		QueryActions* get_current_query_actions() const {return current_query_actions_;}
		Application& get_app() const {return app_;};

	protected:
		std::unique_ptr<CPW::Tools::Route> requested_route_;

	private:
		std::list<CPW::Tools::Route*> routes_list_;
		QueryActions* current_query_actions_;
		Application& app_;
};

#endif // CPW_EXTRAS_DYNAMICELEMENTS_H
