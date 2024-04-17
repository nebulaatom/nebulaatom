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

#include "http/request.h"
#include <optional>
#include <stdexcept>

using namespace Atom::HTTP;

Request::Request(HTTP::RequestType request_type) :
    request_type_(request_type)
{
    http_server_request_ = std::nullopt;
    http_server_response_ = std::nullopt;
}

void Request::AddHeader_(std::string name, std::string value)
{
    response_headers_.push_back(Header(name, value));
}

void Request::SetupResponseHeaders_()
{
    for(auto header : response_headers_)
    {
        if(http_server_response_.has_value())
        {
            http_server_response_.value()->set(header.name, header.value);
        }
        else
            throw std::runtime_error("HTTPServerRequest or HTTPServerResponse cannot be null");
    }
}

void Request::SetupConstRequest_(const Net::HTTPServerRequest& request)
{
    http_server_const_request_.emplace(&request);
    uri_ = request.getURI();
    method_ = request.getMethod();
}

void Request::SetupRequest_(Net::HTTPServerRequest& request)
{
    http_server_request_.emplace(&request);
    uri_ = request.getURI();
    method_ = request.getMethod();
}

void Request::SetupResponse_(Net::HTTPServerResponse& response)
{
    http_server_response_.emplace(&response);
}