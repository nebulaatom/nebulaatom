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
        auto fm = std::make_shared<Filters::FiltersManager>();

        // Setting up the filters

            // fields filter
                fm->get_fields_filter()->get_filter_elements().push_back({"name"});
                fm->get_fields_filter()->get_filter_elements().push_back({"price"});
                fm->get_fields_filter()->get_filter_elements().push_back({"available"});

            // general filter
                Filters::GeneralFilterElement gfe = {Tools::RowValueFormatter(std::string("0, 21")), Filters::GeneralFilterElement::Type::kPageLimit};
                gfe.set_editable(true);
                fm->get_general_filter()->get_filter_elements().insert({"0", std::move(gfe)});

                Filters::GeneralFilterElement gfe2 = {Tools::RowValueFormatter(std::string("newDB")), Filters::GeneralFilterElement::Type::kAs};
                fm->get_general_filter()->get_filter_elements().insert({"1", std::move(gfe2)});

            // iquals filter
                Dynamic::Var val(3);
                Filters::IqualsFilterElement ele = {"id", {val}, Filters::IqualsFilterElement::Type::kNoIqual};
                ele.set_editable(true);
                fm->get_iquals_filter()->get_filter_elements().insert({"0", std::move(ele)});

                Dynamic::Var val2(2);
                Filters::IqualsFilterElement ele2 = {"id_store", {val2}, Filters::IqualsFilterElement::Type::kNoIqual};
                ele.set_editable(true);
                fm->get_iquals_filter()->get_filter_elements().insert({"1", std::move(ele2)});

                Dynamic::Var val3(1);
                Filters::IqualsFilterElement ele3 = {"id_store_category", {val3}, Filters::IqualsFilterElement::Type::kIqual};
                fm->get_iquals_filter()->get_filter_elements().insert({"2", std::move(ele3)});

        // Setting up the function
            Functions::Function f1{"/api/products"};
            f1.get_filters().swap(fm);
            get_functions_manager().get_functions().insert({"/api/products", std::move(f1)});
            routes_list.push_back({"products", "api/products"});

}
