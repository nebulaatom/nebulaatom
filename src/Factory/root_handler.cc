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

#include "Factory/root_handler.h"

using namespace CPW::Factory;

RootHandler::RootHandler()
{

}

RootHandler::~RootHandler()
{

}

bool RootHandler::AuthenticateUser_(HTTPServerRequest& request)
{
	// Read the JSON
		std::string json_body;
		StreamCopier::copyToString(request.stream(), json_body);
		if(json_body.empty())
			return false;

	// Parse JSON
		JSON::Parser parser;
		JSON::Object::Ptr object_json = parser.parse(json_body).extract<JSON::Object::Ptr>();
		Poco::DynamicStruct dynamic_object = *object_json;

	// Verify the key-values
		if(dynamic_object["auth"].isEmpty() || dynamic_object["auth"]["user"].isEmpty() || dynamic_object["auth"]["password"].isEmpty())
			return false;

	// Prepare the row
		User user_params;
		user_params.user = dynamic_object["auth"]["user"].toString();
		user_params.password = dynamic_object["auth"]["password"].toString();
		std::cout << "\nUser: " << user_params.user << ", password: " << user_params.password;

	// Ejecute the query
		bool user_identified = false;
		try
		{
			Poco::Data::MySQL::Connector::registerConnector();
			Session session("MySQL", "host=127.0.0.1;port=3306;db=cpw_woodpecker;user=root;password=mariadb_password;");

			Poco::Data::Statement query(session);
			query << "SELECT * FROM users WHERE username = ? AND password = ?",
				use(user_params.user),
				use(user_params.password)
			;
			query.execute();
			if(query.subTotalRowCount() > 0)
				user_identified = true;
		}
		catch(Poco::Data::MySQL::MySQLException& e)
		{
			std::cout << "\nError: " << e.displayText() << std::endl;
		}
	return user_identified;
}

