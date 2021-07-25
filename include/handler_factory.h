#ifndef HANDLERFACTORY_H
#define HANDLERFACTORY_H

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

#include <map>
#include <functional>

#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "Factory/root_handler.h"

using namespace Poco::Net;

namespace CPW
{

class HandlerFactory : public HTTPRequestHandlerFactory
{
	public:
		HandlerFactory();
		virtual ~HandlerFactory();
};

}

#endif // HANDLERFACTORY_H
