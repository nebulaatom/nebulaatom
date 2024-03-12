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

#ifndef ATOMTOOLS_REQUESTSMANAGER_H
#define ATOMTOOLS_REQUESTSMANAGER_H


#include <string>
#include <map>
#include <functional>
#include <memory>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "http/http_methods.h"


namespace CPW
{
    namespace Tools
    {
        class RequestsManager;
    }
}

using namespace Poco;
using namespace Poco::Net;


class AtomTools::RequestsManager
{
    public:
        using MainFunctor = std::function<void()>;

        RequestsManager();
        ~RequestsManager();

        std::map<std::string, MainFunctor>& get_actions_strings()
        {
            auto& var = actions_strings_;
            return var;
        }
        HTTP::HTTPMethods* get_http_methods(){return http_methods_;}
        void set_http_methods(HTTP::HTTPMethods& http_methods)
        {
            http_methods_ = &http_methods;
        }

    protected:
        void PrepareMethods_();

    private:
        std::map<std::string, MainFunctor> actions_strings_;
        HTTP::HTTPMethods* http_methods_;
};

#endif // ATOMTOOLS_REQUESTSMANAGER_H
