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

#ifndef ATOMQUERY_QUERYACTIONS_H
#define ATOMQUERY_QUERYACTIONS_H


#include <istream>
#include <string>
#include <array>
#include <map>
#include <stdexcept>

#include "Poco/Util/Application.h"
#include <Poco/Net/HTTPServerRequest.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/RecordSet.h>

#include "query/database_manager.h"
#include "tools/manage_json.h"
#include "filters/filters_manager.h"
#include "http/common_responses.h"
#include "tools/row_value_formatter.h"
#include "query/results.h"


namespace CPW
{
    namespace Query
    {
        class QueryActions;
    }
}


class AtomQuery::QueryActions
{
    public:
        QueryActions();
        ~QueryActions();
};


#endif // ATOMQUERY_QUERYACTIONS_H
