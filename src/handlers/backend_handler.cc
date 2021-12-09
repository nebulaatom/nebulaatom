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
	get_dynamic_elements()->get_routes_list()->push_back({"business",std::vector<std::string>{"api", get_api_verion(), "business"}});
	get_dynamic_elements()->get_routes_list()->push_back({"levels",std::vector<std::string>{"api", get_api_verion(), "business", "levels"}});
	get_dynamic_elements()->get_routes_list()->push_back({"levels_log",std::vector<std::string>{"api", get_api_verion(), "business", "levels", "log"}});
	get_dynamic_elements()->get_routes_list()->push_back({"banks_accounts",std::vector<std::string>{"api", get_api_verion(), "business", "banks_accounts"}});
	get_dynamic_elements()->get_routes_list()->push_back({"transactions",std::vector<std::string>{"api", get_api_verion(), "business", "banks_accounts", "transactions"}});

	get_dynamic_elements()->get_routes_list()->push_back({"users",std::vector<std::string>{"api", get_api_verion(), "users"}});
	get_dynamic_elements()->get_routes_list()->push_back({"settings",std::vector<std::string>{"api", get_api_verion(), "users", "settings"}});
	get_dynamic_elements()->get_routes_list()->push_back({"actions",std::vector<std::string>{"api", get_api_verion(), "users", "actions"}});
	get_dynamic_elements()->get_routes_list()->push_back({"permissions",std::vector<std::string>{"api", get_api_verion(), "users", "permissions"}});
	get_dynamic_elements()->get_routes_list()->push_back({"permissions_log",std::vector<std::string>{"api", get_api_verion(), "users", "permissions", "log"}});

	get_dynamic_elements()->get_routes_list()->push_back({"accounts",std::vector<std::string>{"api", get_api_verion(), "accounts"}});
	get_dynamic_elements()->get_routes_list()->push_back({"phones",std::vector<std::string>{"api", get_api_verion(), "accounts", "phones"}});
	get_dynamic_elements()->get_routes_list()->push_back({"websites",std::vector<std::string>{"api", get_api_verion(), "accounts", "websites"}});
	get_dynamic_elements()->get_routes_list()->push_back({"emails",std::vector<std::string>{"api", get_api_verion(), "accounts", "emails"}});
	get_dynamic_elements()->get_routes_list()->push_back({"addresses",std::vector<std::string>{"api", get_api_verion(), "accounts", "addresses"}});

	get_dynamic_elements()->get_routes_list()->push_back({"deals",std::vector<std::string>{"api", get_api_verion(), "deals"}});
	get_dynamic_elements()->get_routes_list()->push_back({"tasks",std::vector<std::string>{"api", get_api_verion(), "deals", "tasks"}});
	get_dynamic_elements()->get_routes_list()->push_back({"messages",std::vector<std::string>{"api", get_api_verion(), "deals", "messages"}});
	get_dynamic_elements()->get_routes_list()->push_back({"meetings",std::vector<std::string>{"api", get_api_verion(), "deals", "meetings"}});
	get_dynamic_elements()->get_routes_list()->push_back({"calls",std::vector<std::string>{"api", get_api_verion(), "deals", "calls"}});

	get_dynamic_elements()->get_routes_list()->push_back({"pays",std::vector<std::string>{"api", get_api_verion(), "pays"}});
	get_dynamic_elements()->get_routes_list()->push_back({"transactions",std::vector<std::string>{"api", get_api_verion(), "pays", "transactions"}});
	get_dynamic_elements()->get_routes_list()->push_back({"references",std::vector<std::string>{"api", get_api_verion(), "pays", "references"}});

	get_dynamic_elements()->get_routes_list()->push_back({"invoices",std::vector<std::string>{"api", get_api_verion(), "invoices"}});
	get_dynamic_elements()->get_routes_list()->push_back({"invoice_items",std::vector<std::string>{"api", get_api_verion(), "invoices", "items"}});
	get_dynamic_elements()->get_routes_list()->push_back({"taxes",std::vector<std::string>{"api", get_api_verion(), "invoices", "taxes"}});
	get_dynamic_elements()->get_routes_list()->push_back({"taxes_log",std::vector<std::string>{"api", get_api_verion(), "invoices", "taxes", "log"}});
	get_dynamic_elements()->get_routes_list()->push_back({"withholdings",std::vector<std::string>{"api", get_api_verion(), "invoices", "withholdings"}});
	get_dynamic_elements()->get_routes_list()->push_back({"withholdings_log",std::vector<std::string>{"api", get_api_verion(), "invoices", "withholdings", "log"}});

	get_dynamic_elements()->get_routes_list()->push_back({"inventories",std::vector<std::string>{"api", get_api_verion(), "inventories"}});
	get_dynamic_elements()->get_routes_list()->push_back({"inventory_items",std::vector<std::string>{"api", get_api_verion(), "inventories", "items"}});
	get_dynamic_elements()->get_routes_list()->push_back({"inventory_items_log",std::vector<std::string>{"api", get_api_verion(), "inventories", "items", "log"}});
	get_dynamic_elements()->get_routes_list()->push_back({"requirements",std::vector<std::string>{"api", get_api_verion(), "inventories", "items", "requirements"}});

	get_dynamic_elements()->get_routes_list()->push_back({"currency",std::vector<std::string>{"api", get_api_verion(), "currency"}});
	get_dynamic_elements()->get_routes_list()->push_back({"exchange_rate",std::vector<std::string>{"api", get_api_verion(), "currency", "exchange_rate"}});
	get_dynamic_elements()->get_routes_list()->push_back({"exchange_rate_log",std::vector<std::string>{"api", get_api_verion(), "currency", "exchange_rate", "log"}});
}
