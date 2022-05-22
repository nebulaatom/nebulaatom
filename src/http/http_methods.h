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

#ifndef CPW_HTTP_HTTPMETHODS_H
#define CPW_HTTP_HTTPMETHODS_H


#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "extras/dynamic_elements.h"
#include "http/common_responses.h"


namespace CPW
{
    namespace HTTP
    {
        class HTTPMethods;
    }
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


class CPW::HTTP::HTTPMethods
{
    public:
        using DynamicElementSharedPtr = std::shared_ptr<Extras::DynamicElements>;

        HTTPMethods();
        virtual ~HTTPMethods();

        DynamicElementSharedPtr get_dynamic_elements() const {return dynamic_elements_;}
        void set_dynamic_elements(DynamicElementSharedPtr dynamic_elements) {dynamic_elements_ = dynamic_elements;}

        virtual void HandleGETMethod_();
        virtual void HandlePOSTMethod_();
        virtual void HandlePUTMethod_();
        virtual void HandleDELMethod_();

    protected:
        bool QueryProcess_(Query::TypeAction action);

    private:
        DynamicElementSharedPtr dynamic_elements_;
        CommonResponses responses_;
};

#endif // CPW_HTTP_HTTPMETHODS_H
