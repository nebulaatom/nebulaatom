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

#include "Factory/business_handler.h"

using namespace CPW::Factory;

BusinessHandler::~BusinessHandler()
{

}

void BusinessHandler::AddRoutes_()
{
	get_routes_list().push_back(new Route("business",std::vector<std::string>{"api", get_api_verion(), "business"}));
	get_routes_list().push_back(new Route("levels",std::vector<std::string>{"api", get_api_verion(), "business", "levels"}));
	get_routes_list().push_back(new Route("levels_log",std::vector<std::string>{"api", get_api_verion(), "business", "levels", "log"}));
	get_routes_list().push_back(new Route("banks_accounts",std::vector<std::string>{"api", get_api_verion(), "business", "banks_accounts"}));
	get_routes_list().push_back(new Route("transactions",std::vector<std::string>{"api", get_api_verion(), "business", "banks_accounts", "transactions"}));

	get_routes_list().push_back(new Route("users",std::vector<std::string>{"api", get_api_verion(), "users"}));
	get_routes_list().push_back(new Route("settings",std::vector<std::string>{"api", get_api_verion(), "users", "settings"}));
	get_routes_list().push_back(new Route("actions",std::vector<std::string>{"api", get_api_verion(), "users", "actions"}));
	get_routes_list().push_back(new Route("permissions",std::vector<std::string>{"api", get_api_verion(), "users", "permissions"}));
	get_routes_list().push_back(new Route("permissions_log",std::vector<std::string>{"api", get_api_verion(), "users", "permissions", "log"}));

	get_routes_list().push_back(new Route("accounts",std::vector<std::string>{"api", get_api_verion(), "accounts"}));
	get_routes_list().push_back(new Route("phones",std::vector<std::string>{"api", get_api_verion(), "accounts", "phones"}));
	get_routes_list().push_back(new Route("websites",std::vector<std::string>{"api", get_api_verion(), "accounts", "websites"}));
	get_routes_list().push_back(new Route("emails",std::vector<std::string>{"api", get_api_verion(), "accounts", "emails"}));
	get_routes_list().push_back(new Route("addresses",std::vector<std::string>{"api", get_api_verion(), "accounts", "addresses"}));

	get_routes_list().push_back(new Route("deals",std::vector<std::string>{"api", get_api_verion(), "deals"}));
	get_routes_list().push_back(new Route("tasks",std::vector<std::string>{"api", get_api_verion(), "deals", "tasks"}));
	get_routes_list().push_back(new Route("messages",std::vector<std::string>{"api", get_api_verion(), "deals", "messages"}));
	get_routes_list().push_back(new Route("meetings",std::vector<std::string>{"api", get_api_verion(), "deals", "meetings"}));
	get_routes_list().push_back(new Route("calls",std::vector<std::string>{"api", get_api_verion(), "deals", "calls"}));

	get_routes_list().push_back(new Route("pays",std::vector<std::string>{"api", get_api_verion(), "pays"}));
	get_routes_list().push_back(new Route("transactions",std::vector<std::string>{"api", get_api_verion(), "pays", "transactions"}));
	get_routes_list().push_back(new Route("references",std::vector<std::string>{"api", get_api_verion(), "pays", "references"}));

	get_routes_list().push_back(new Route("invoices",std::vector<std::string>{"api", get_api_verion(), "invoices"}));
	get_routes_list().push_back(new Route("invoice_items",std::vector<std::string>{"api", get_api_verion(), "invoices", "items"}));
	get_routes_list().push_back(new Route("taxes",std::vector<std::string>{"api", get_api_verion(), "invoices", "taxes"}));
	get_routes_list().push_back(new Route("taxes_log",std::vector<std::string>{"api", get_api_verion(), "invoices", "taxes", "log"}));
	get_routes_list().push_back(new Route("withholdings",std::vector<std::string>{"api", get_api_verion(), "invoices", "withholdings"}));
	get_routes_list().push_back(new Route("withholdings_log",std::vector<std::string>{"api", get_api_verion(), "invoices", "withholdings", "log"}));

	get_routes_list().push_back(new Route("inventories",std::vector<std::string>{"api", get_api_verion(), "inventories"}));
	get_routes_list().push_back(new Route("inventory_items",std::vector<std::string>{"api", get_api_verion(), "inventories", "items"}));
	get_routes_list().push_back(new Route("inventory_items_log",std::vector<std::string>{"api", get_api_verion(), "inventories", "items", "log"}));
	get_routes_list().push_back(new Route("requirements",std::vector<std::string>{"api", get_api_verion(), "inventories", "items", "requirements"}));

	get_routes_list().push_back(new Route("currency",std::vector<std::string>{"api", get_api_verion(), "currency"}));
	get_routes_list().push_back(new Route("exchange_rate",std::vector<std::string>{"api", get_api_verion(), "currency", "exchange_rate"}));
	get_routes_list().push_back(new Route("exchange_rate_log",std::vector<std::string>{"api", get_api_verion(), "currency", "exchange_rate", "log"}));
}
