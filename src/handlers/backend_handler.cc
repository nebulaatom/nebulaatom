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

	routes_list.push_back({"business", {"api", get_api_verion(), "business"}});
	routes_list.push_back({"levels", {"api", get_api_verion(), "business", "levels"}});
	routes_list.push_back({"levels_log", {"api", get_api_verion(), "business", "levels", "log"}});
	routes_list.push_back({"banks_accounts", {"api", get_api_verion(), "business", "banks_accounts"}});
	routes_list.push_back({"transactions", {"api", get_api_verion(), "business", "banks_accounts", "transactions"}});

	routes_list.push_back({"users", {"api", get_api_verion(), "users"}});
	routes_list.push_back({"settings", {"api", get_api_verion(), "users", "settings"}});
	routes_list.push_back({"actions", {"api", get_api_verion(), "users", "actions"}});
	routes_list.push_back({"permissions", {"api", get_api_verion(), "users", "permissions"}});
	routes_list.push_back({"permissions_log", {"api", get_api_verion(), "users", "permissions", "log"}});

	routes_list.push_back({"accounts", {"api", get_api_verion(), "accounts"}});
	routes_list.push_back({"phones", {"api", get_api_verion(), "accounts", "phones"}});
	routes_list.push_back({"websites", {"api", get_api_verion(), "accounts", "websites"}});
	routes_list.push_back({"emails", {"api", get_api_verion(), "accounts", "emails"}});
	routes_list.push_back({"addresses", {"api", get_api_verion(), "accounts", "addresses"}});

	routes_list.push_back({"deals", {"api", get_api_verion(), "deals"}});
	routes_list.push_back({"tasks", {"api", get_api_verion(), "deals", "tasks"}});
	routes_list.push_back({"messages", {"api", get_api_verion(), "deals", "messages"}});
	routes_list.push_back({"meetings", {"api", get_api_verion(), "deals", "meetings"}});
	routes_list.push_back({"calls", {"api", get_api_verion(), "deals", "calls"}});

	routes_list.push_back({"pays", {"api", get_api_verion(), "pays"}});
	routes_list.push_back({"transactions", {"api", get_api_verion(), "pays", "transactions"}});
	routes_list.push_back({"references", {"api", get_api_verion(), "pays", "references"}});

	routes_list.push_back({"invoices", {"api", get_api_verion(), "invoices"}});
	routes_list.push_back({"invoice_items", {"api", get_api_verion(), "invoices", "items"}});
	routes_list.push_back({"taxes", {"api", get_api_verion(), "invoices", "taxes"}});
	routes_list.push_back({"taxes_log", {"api", get_api_verion(), "invoices", "taxes", "log"}});
	routes_list.push_back({"withholdings", {"api", get_api_verion(), "invoices", "withholdings"}});
	routes_list.push_back({"withholdings_log", {"api", get_api_verion(), "invoices", "withholdings", "log"}});

	routes_list.push_back({"inventories", {"api", get_api_verion(), "inventories"}});
	routes_list.push_back({"inventory_items", {"api", get_api_verion(), "inventories", "items"}});
	routes_list.push_back({"inventory_items_log", {"api", get_api_verion(), "inventories", "items", "log"}});
	routes_list.push_back({"requirements", {"api", get_api_verion(), "inventories", "items", "requirements"}});

	routes_list.push_back({"currency", {"api", get_api_verion(), "currency"}});
	routes_list.push_back({"exchange_rate", {"api", get_api_verion(), "currency", "exchange_rate"}});
	routes_list.push_back({"exchange_rate_log", {"api", get_api_verion(), "currency", "exchange_rate", "log"}});
}
