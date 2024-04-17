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


namespace Atom
{
    namespace HTTP
    {
        enum class RequestType;
        class Header;
        class Request;
    }
}

using namespace Poco;
using namespace Poco::Net;


enum class Atom::HTTP::RequestType
{
    kRequest
    ,kConstRequest
};

class Atom::HTTP::Header
{
    public:
        Header(const std::string name, const std::string value) : 
            name(name)
            ,value(value)
        {}
        virtual ~Header(){}

        const std::string name;
        const std::string value;
};

class Atom::HTTP::Request
{
    public:
        using HTTPServerConstRequestPtr = std::optional<const HTTPServerRequest*>;
        using HTTPServerRequestPtr = std::optional<HTTPServerRequest*>;
        using HTTPServerResponsePtr = std::optional<HTTPServerResponse*>;

        Request(RequestType request_type);
        
        RequestType get_request_type() const { return request_type_; }
        HTTPServerConstRequestPtr& get_http_server_const_request()
        {
            auto& var = http_server_const_request_;
            return var;
        }
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
        std::vector<Header>& get_response_headers()
        {
            auto& var = response_headers_;
            return var;
        }
        std::string get_uri() const { return uri_; }
        std::string get_method() const { return method_; }

        void set_request_type(RequestType request_type) { request_type_ = request_type; }
        void set_uri(std::string uri) { uri_ = uri; }
        void set_method(std::string method) { method_ = method; }
        
        void AddHeader_(std::string name, std::string value);
        void SetupResponseHeaders_();

    protected:
        void SetupConstRequest_(const Net::HTTPServerRequest& request);
        void SetupRequest_(Net::HTTPServerRequest& request);
        void SetupResponse_(Net::HTTPServerResponse& response);

    private:
        RequestType request_type_;
        HTTPServerConstRequestPtr http_server_const_request_;
        HTTPServerRequestPtr http_server_request_;
        HTTPServerResponsePtr http_server_response_;
        std::vector<Header> response_headers_;
        std::string uri_;
        std::string method_;

};

#endif // ATOM_HTTP_REQUEST
