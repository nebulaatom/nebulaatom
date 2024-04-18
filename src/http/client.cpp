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

#include "http/client.h"

using namespace Atom::HTTP;

Client::Client(std::string uri) :
    uri_(uri)
    ,username_("")
    ,password_("")
    ,use_credentials_(false)
{
    
}

Client::Client(std::string uri, std::string username, std::string password) :
    uri_(uri)
    ,username_(username)
    ,password_(password)
    ,use_credentials_(true)
{
    
}

void Client::SendRequest_()
{
	try
	{
		URI uri(uri_);
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";

		Net::HTTPClientSession session(uri.getHost(), uri.getPort());
		Net::HTTPRequest http_request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		Net::HTTPResponse http_response;

        if(use_credentials_)
        {
            Net::HTTPCredentials credentials(username_, password_);
            credentials.authenticate(http_request, http_response);
        }

        session.sendRequest(http_request);
        std::istream& is = session.receiveResponse(http_response);
        std::stringstream ss;
		StreamCopier::copyStream(is, ss);
        response_handler_(ss, http_request, http_response);
	}
	catch (Exception& exc)
	{
		Tools::OutputLogger::Log_(exc.displayText());
		return;
	}
}
