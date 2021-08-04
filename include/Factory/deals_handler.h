/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2021  <copyright holder> <email>
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

#ifndef CPW_FACTORY_DEALSHANDLER_H
#define CPW_FACTORY_DEALSHANDLER_H

#include "root_handler.h"

namespace CPW {
namespace Factory {

/**
 * @todo write docs
 */
class DealsHandler : public CPW::QueryActions,  public CPW::Factory::RootHandler
{
public:
    /**
     * Default constructor
     */
    DealsHandler();

    /**
     * Destructor
     */
    ~DealsHandler();

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

#endif // CPW_FACTORY_DEALSHANDLER_H
