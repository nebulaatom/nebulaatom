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

#include "handlers/backend_handler.h"

using namespace CPW::Handlers;

BackendHandler::~BackendHandler()
{

}

void BackendHandler::AddRoutes_()
{
    auto& routes_list = get_dynamic_elements()->get_routes_list();

    // Function /api/products
        // Setting up the filters
            auto fm = std::make_shared<Filters::FiltersManager>();
            // fields filter
            fm->get_fields_filter()->get_filter_elements().push_back({"name"});
            // iquals filter
            Dynamic::Var val(-1);
            fm->get_iquals_filter()->get_filter_elements().push_back({"id", {val}, "no-iqual"});

        // Setting up the function
            Functions::Function f1{"/api/products"};
            f1.get_filters().swap(fm);
            get_functions_manager().get_functions().insert({"/api/products", std::move(f1)});
            routes_list.push_back({"products", "api/products"});

    // Function /api/stores
        // Setting up the filters
            auto fm2 = std::make_shared<Filters::FiltersManager>();
            // fields filter
            fm2->get_fields_filter()->get_filter_elements().push_back({"name"});
            // iquals filter
            Dynamic::Var val2(-1);
            fm2->get_iquals_filter()->get_filter_elements().push_back({"id", {val2}, "no-iqual"});

        // Setting up Iqual parameterized
            auto found = fm2->get_filters_parameterized().find(Filters::FilterType::kIqual);
            if(found != fm2->get_filters_parameterized().end())
                fm2->get_filters_parameterized()[Filters::FilterType::kIqual] = true;

        // Setting up the function
            Functions::Function f2{"/api/stores"};
            f2.get_filters().swap(fm2);
            get_functions_manager().get_functions().insert({"/api/stores", std::move(f2)});
            routes_list.push_back({"stores", "api/stores"});
}
