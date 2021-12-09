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

#ifndef CPW_EXTRAS_HTTPMETHODS_H
#define CPW_EXTRAS_HTTPMETHODS_H


#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "extras/dynamic_elements.h"


namespace CPW
{
	namespace Extras
	{
		class HTTPMethods;
	}
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


class CPW::Extras::HTTPMethods
{
	public:
		HTTPMethods();
		~HTTPMethods();

		std::shared_ptr<Extras::DynamicElements> get_dynamic_elements() const {return dynamic_elements_;}
		void set_dynamic_elements(std::shared_ptr<Extras::DynamicElements> dynamic_elements) {dynamic_elements_ = dynamic_elements;}

		virtual void HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response);
		virtual void HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response);
		virtual void HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response);
		virtual void HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response);

	private:
		std::shared_ptr<Extras::DynamicElements> dynamic_elements_;
};

#endif // CPW_EXTRAS_HTTPMETHODS_H
