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

#ifndef ATOM_HTTP_REQUEST
#define ATOM_HTTP_REQUEST


#include <vector>
#include <optional>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPCookie.h>


namespace Atom
{
    namespace HTTP
    {
        class Header;
        class Cookie;
        class Request;
    }
}

using namespace Poco;
using namespace Poco::Net;



class Atom::HTTP::Header
{
    public:
        Header(std::string name, std::string value) : 
            name(name)
            ,value(value)
        {}
        virtual ~Header(){}

        std::string name;
        std::string value;
};

class Atom::HTTP::Cookie
{
    public:
        Cookie(std::string name, std::string value) : 
            name(name)
            ,value(value)
        {}
        virtual ~Cookie(){}

        std::string name;
        std::string value;
};

class Atom::HTTP::Request
{
    public:
        using HTTPServerRequestPtr = std::optional<HTTPServerRequest*>;
        using HTTPServerResponsePtr = std::optional<HTTPServerResponse*>;

        Request();
        
        HTTPServerRequestPtr& get_http_server_request()
        {
            auto& var = http_server_request_;
            return var;
        }
        HTTPServerResponsePtr& get_http_server_response()
        {
            auto& var = http_server_response_;
            return var;
        }
        std::vector<HTTP::Header> get_headers()
        {
            auto& var = headers_;
            return var;
        }
        std::vector<HTTP::Cookie> get_cookies()
        {
            auto& var = cookies_;
            return var;
        }
        std::string get_uri() const { return uri_; }
        std::string get_method() const { return method_; }

        void set_uri(std::string uri) { uri_ = uri; }
        void set_method(std::string method) { method_ = method; }
        
        void AddHeader_(std::string name, std::string value);
        void AddCookie_(std::string name, std::string value);

    protected:
        void SetupRequest_(Net::HTTPServerRequest& request);
        void SetupResponse_(Net::HTTPServerResponse& response);
        void SetupHeaders_();
        void SetupCookies_();

    private:
        HTTPServerRequestPtr http_server_request_;
        HTTPServerResponsePtr http_server_response_;
        std::vector<HTTP::Header> headers_;
        std::vector<HTTP::Cookie> cookies_;
        std::string uri_;
        std::string method_;

};

#endif // ATOM_HTTP_REQUEST
