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

#ifndef CPW_FACTORY_CURRENCYHANDLER_H
#define CPW_FACTORY_CURRENCYHANDLER_H

#include <../../home/josefelixrc7/Documents/cpwonline/solutions/cpw-woodpecker-server/src/query_actions.h>
#include "root_handler.h"

namespace CPW {
namespace Factory {

/**
 * @todo write docs
 */
class CurrencyHandler :  CPW::QueryActions,   CPW::Factory::RootHandler
{
public:
    /**
     * Default constructor
     */
    CurrencyHandler();

    /**
     * Destructor
     */
    ~CurrencyHandler();

    /**
     * @todo write docs
     *
     * @param request TODO
     * @param response TODO
     * @return TODO
     */
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

protected:
    /**
     * @todo write docs
     *
     * @param request TODO
     * @param response TODO
     * @return TODO
     */
    virtual void HandleDELMethod_(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) = 0;

    /**
     * @todo write docs
     *
     * @param request TODO
     * @param response TODO
     * @return TODO
     */
    virtual void HandlePUTMethod_(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) = 0;

    /**
     * @todo write docs
     *
     * @param request TODO
     * @param response TODO
     * @return TODO
     */
    virtual void HandlePOSTMethod_(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) = 0;

    /**
     * @todo write docs
     *
     * @param request TODO
     * @param response TODO
     * @return TODO
     */
    virtual void HandleGETMethod_(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) = 0;

};

}}

#endif // CPW_FACTORY_CURRENCYHANDLER_H
