/*
* Nebula Atom

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

#include "handlers/websocket_handler.h"

using namespace Atom::Handlers;

WebSocketHandler::WebSocketHandler()
{

}

WebSocketHandler::~WebSocketHandler()
{

}

void WebSocketHandler::Process_()
{
    auto& request = get_http_server_request().value();
    auto& response = get_http_server_response().value();

    try
    {
        websocket_ = std::make_unique<WebSocket>(*request, *response);
        websocket_->setReceiveTimeout(Poco::Timespan());
        Tools::OutputLogger::Log_("WebSocket connection established.");

        HandleNewConnection_(*this, *this);
        Transfer_();
        HandleConnectionClosed_(*this);
    }
    catch (WebSocketException& error)
    {
        Tools::OutputLogger::Log_(error.displayText());
        switch (error.code())
        {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                response->set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                response->setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                response->setContentLength(0);
                response->send();
                break;
        }
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Log_("Error on websocket_handler.cpp on Process_(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
}

void WebSocketHandler::Transfer_()
{
    char buffer[1024];
    int flags, n;

    do
    {
        n = websocket_->receiveFrame(buffer, sizeof(buffer), flags);
        if(n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE)
        {
            if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING)
                websocket_->sendFrame(buffer, n, WebSocket::FRAME_OP_PONG);
            else
            {
                std::string message(buffer, n);
                HandleNewMessage_(*this, message);
            }
        }
    }
    while (n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);

    Tools::OutputLogger::Log_("WebSocket connection closed.");

}

void WebSocketHandler::Send_(std::string message) const
{
    char buffer[1024];
    int n = 0;
    for(auto it : message)
    {
        buffer[n] = it;
        n++;
    }
    websocket_->sendFrame(buffer, n);
}