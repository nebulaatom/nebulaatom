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

#include "query_actions.h"

using namespace CPW;

QueryActions::QueryActions() :
	session_((Data::MySQL::Connector::registerConnector(), Data::Session("MySQL", "host=127.0.0.1;port=3306;db=cpw_woodpecker;user=root;password=mariadb_password;"))),
	query_(session_)
{
	table_rows_ = new std::map<std::string, std::string>;
	result_json_ = new Poco::JSON::Array;
	current_filters_.limit = "20";
	current_filters_.page = "0";
}

QueryActions::~QueryActions()
{

}

void QueryActions::ResetQuery_()
{
	query_.reset(session_);
}
