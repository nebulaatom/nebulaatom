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
#include "handlers/root_handler.h"
#include <memory>

using namespace Atom::Handlers;

WebSocketHandler::WebSocketHandler(std::vector<WebSocketHandler*>& connected_sockets) :
    RootHandler()
    ,connected_sockets_(connected_sockets)
{

}

WebSocketHandler::~WebSocketHandler()
{

}

void WebSocketHandler::AddFunctions_()
{

}

void WebSocketHandler::Process_()
{
    Application& app = Application::instance();
    try
    {
        websocket_ = std::make_unique<WebSocket>(*get_request(), *get_response());
        connected_sockets_.push_back(this);
        websocket_->setReceiveTimeout(Poco::Timespan());
        app.logger().information("-- WebSocket connection established.");
        char buffer[1024];
        int flags;
        int n;

        do
        {
            n = websocket_->receiveFrame(buffer, sizeof(buffer), flags);
            if(n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE)
            {
                if ((flags & WebSocket::FRAME_OP_BITMASK) == WebSocket::FRAME_OP_PING)
                    websocket_->sendFrame(buffer, n, WebSocket::FRAME_OP_PONG);
                else
                {
                    // Transfer
                    for(auto it : connected_sockets_)
                    {
                        it->websocket_->sendFrame(buffer, n, flags);
                    }
                }
            }
        }
        while (n > 0 && (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
        app.logger().information("-- WebSocket connection closed.");

        for (auto it = connected_sockets_.begin() ; it != connected_sockets_.end(); ++it)
        {
            if (*it == this)
            {
                connected_sockets_.erase(it);
                app.logger().information("Connection closed %s", websocket_->peerAddress().toString());
                break;
            }
        }
    }
    catch (WebSocketException& exc)
    {
        app.logger().log(exc);
        switch (exc.code())
        {
            case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
                get_response()->set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
            case WebSocket::WS_ERR_NO_HANDSHAKE:
            case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
            case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
                get_response()->setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
                get_response()->setContentLength(0);
                get_response()->send();
                break;
        }
    }
    catch(std::exception& error)
    {
        app.logger().information("Error: " + std::string(error.what()));
    }
}

void WebSocketHandler::HandleGETMethod_()
{
    
}

void WebSocketHandler::HandlePOSTMethod_()
{
    
}

void WebSocketHandler::HandlePUTMethod_()
{
    
}

void WebSocketHandler::HandleDELMethod_()
{
    
}
