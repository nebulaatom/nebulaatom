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
#include <Poco/Net/NetSSL.h>
#include <Poco/Net/SSLException.h>

using namespace Atom::HTTP;

Client::Client(std::string uri, const std::string method) :
    use_ssl_(false)
    ,uri_(uri)
    ,method_(method)
    ,username_("")
    ,password_("")
    ,use_credentials_(false)
    ,ssl_context_(nullptr)
{
    
}

void Client::UseCredentials_(std::string username, std::string password)
{
    username_ = username;
    password_ = password;
}

void Client::SetupSSL_(std::string key, std::string cert)
{
    use_ssl_ = true;
    /*Net::initializeSSL();
    SharedPtr<Net::InvalidCertificateHandler> cert_handler = new AcceptCertificateHandler(false);*/
    ssl_context_ = new Context(Context::CLIENT_USE, key, cert, "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    //SSLManager::instance().initializeClient(0, cert_handler, ssl_context_);
}

void Client::SetupSSL_(std::string rootcert)
{
    use_ssl_ = true;
    /*Net::initializeSSL();
    SharedPtr<Net::InvalidCertificateHandler> cert_handler = new AcceptCertificateHandler(false);*/
    ssl_context_ = new Context(Context::CLIENT_USE, "", "", rootcert, Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    //SSLManager::instance().initializeClient(0, cert_handler, ssl_context_);
}

void Client::AddHeader_(std::string name, std::string value)
{
    
}

void Client::AddCookie_(std::string name, std::string value)
{
    
}

void Client::SendRequest_()
{
    if(use_ssl_) SendSSLRequest_();
    else SendNormalRequest_();
}

void Client::SendNormalRequest_()
{
	try
	{
		URI uri(uri_);
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";

		Net::HTTPClientSession session(uri.getHost(), uri.getPort());
		Net::HTTPRequest http_request(method_, path, HTTPMessage::HTTP_1_1);
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

void Client::SendSSLRequest_()
{
	try
	{
        Net::initializeSSL();
		URI uri(uri_);
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";

		Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), ssl_context_);
		Net::HTTPRequest http_request(method_, path, HTTPMessage::HTTP_1_1);
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

        Net::uninitializeSSL();
	}
	catch (SSLException& exc)
	{
		Tools::OutputLogger::Log_(exc.displayText());
		return;
	}
	catch (Exception& exc)
	{
		Tools::OutputLogger::Log_(exc.displayText());
		return;
	}
}
