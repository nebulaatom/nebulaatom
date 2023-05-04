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

    routes_list.push_back({"products", {"api", get_api_version(), "products"}});
    routes_list.push_back({"stores", {"api", get_api_version(), "stores"}});
    routes_list.push_back({"store_categories", {"api", get_api_version(), "stores", "categories"}});

    /*routes_list.push_back({"business", {"api", get_api_version(), "business"}});
    routes_list.push_back({"levels", {"api", get_api_version(), "business", "levels"}});
    routes_list.push_back({"levels_log", {"api", get_api_version(), "business", "levels", "log"}});
    routes_list.push_back({"banks_accounts", {"api", get_api_version(), "business", "banks_accounts"}});
    routes_list.push_back({"transactions", {"api", get_api_version(), "business", "banks_accounts", "transactions"}});*/
}
