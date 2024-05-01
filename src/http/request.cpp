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

using namespace Atom::HTTP;

Request::Request() :
    uri_("")
    ,method_("")
    ,content_type_("")
{
    http_server_request_ = std::nullopt;
    http_server_response_ = std::nullopt;
}

void Request::AddHeader_(std::string name, std::string value)
{
    headers_.push_back(Header(name, value));
}

void Request::AddCookie_(std::string name, std::string value)
{
    cookies_.push_back(Cookie(name, value));
}

void Request::SetupRequest_(Net::HTTPServerRequest& request)
{
    http_server_request_.emplace(&request);
    uri_ = request.getURI();
    method_ = request.getMethod();

    Net::MessageHeader::splitParameters(request.getContentType(), content_type_, content_type_parameters_);
}

void Request::SetupResponse_(Net::HTTPServerResponse& response)
{
    http_server_response_.emplace(&response);
}

void Request::SetupHeaders_()
{
    for(auto& header : headers_)
        http_server_response_.value()->add(header.name, header.value);
}

void Request::SetupCookies_()
{
    for(auto& cookie : cookies_)
    {
        HTTPCookie poco_cookie(cookie.name, cookie.value);
        http_server_response_.value()->addCookie(poco_cookie);
    }
}