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

#ifndef CPW_FACTORY_ACCOUNTSHANDLER_H
#define CPW_FACTORY_ACCOUNTSHANDLER_H

#include "query_handler.h"
#include "Factory/root_handler.h"

namespace CPW
{
namespace Factory
{


class AccountsHandler : public CPW::QueryActions, CPW::Factory::RootHandler
{
	public:
		AccountsHandler();
		~AccountsHandler();
		void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

	protected:
		virtual void HandleDELMethod_(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		virtual void HandlePUTMethod_(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		virtual void HandlePOSTMethod_(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		virtual void HandleGETMethod_(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

};

}}

#endif // CPW_FACTORY_ACCOUNTSHANDLER_H
