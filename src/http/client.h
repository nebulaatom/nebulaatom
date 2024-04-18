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

#ifndef ATOM_HTTP_CLIENT
#define ATOM_HTTP_CLIENT


#include <functional>
#include <vector>
#include <optional>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

#include "tools/output_logger.h"


namespace Atom
{
    namespace HTTP
    {
        class Client;
    }
}

using namespace Poco;
using namespace Poco::Net;


class Atom::HTTP::Client
{
    public:
        using ClientResponseFunction = std::function<void(std::stringstream& response, Net::HTTPRequest& http_request, Net::HTTPResponse& http_response)>;

        Client(std::string uri);
        Client(std::string uri, std::string username, std::string password);

        ClientResponseFunction get_response_handler() const { return response_handler_; }

        void set_response_handler(ClientResponseFunction response_handler) { response_handler_ = response_handler; }

        void SendRequest_();
        void SendRequest_(std::string username, std::string password);

    private:
        std::string uri_;
        std::string username_;
        std::string password_;
        bool use_credentials_;
        ClientResponseFunction response_handler_;
};

#endif // ATOM_HTTP_CLIENT
