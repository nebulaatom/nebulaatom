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

void BusinessHandler::HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
	get_current_query_actions()->IdentifyFilters_(request);
	get_current_query_actions()->ComposeQuery_(QueryActions::TypeAction::kSelect, get_table_route(), "");
	get_current_query_actions()->ExecuteQuery_();

	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	get_current_query_actions()->get_result_json()->stringify(out);
	out.flush();

	/*Poco::JSON::Array array_json;
	try
	{
		Poco::Data::MySQL::Connector::registerConnector();
		Session session("MySQL", "host=127.0.0.1;port=3306;db=cpw_woodpecker;user=root;password=mariadb_password;");

		BusinessRow business;
		Poco::Data::Statement select(session);
		select << "SELECT * FROM business",
			into(business.id),
			into(business.name),
			into(business.image),
			into(business.reg_date),
			range(0, 1)
		;

		int a = 0;
		while (!select.done())
		{
			select.execute();

			Poco::JSON::Object::Ptr object_json = new Poco::JSON::Object;
			object_json->set("id", business.id);
			object_json->set("name", business.name);
			object_json->set("imagen", business.image);
			object_json->set("reg_date", business.reg_date);
			array_json.set(a, object_json);
			a++;
		}
	}
	catch(Poco::Data::MySQL::MySQLException& e)
	{
		std::cout << "\nError: " << e.displayText() << std::endl;
	}
	response.setStatus(HTTPResponse::HTTP_OK);
	response.setContentType("application/json");

	std::ostream& out = response.send();
	array_json.stringify(out);
	out.flush();*/
}

void BusinessHandler::HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{
}

void BusinessHandler::HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response)
{

}


}
