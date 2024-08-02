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

#ifndef NAF_HANDLERS_WEBSOCKETHANDLER
#define NAF_HANDLERS_WEBSOCKETHANDLER


#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"

#include "handlers/root_handler.h"
#include "http/request.h"


namespace NAF
{
    namespace Handlers
    {
        class WebSocketHandler;
    }
}


class NAF::Handlers::WebSocketHandler :
    public RootHandler
{
    public:
        using VectorSocket = std::vector<const WebSocketHandler*>;

        WebSocketHandler();
        virtual ~WebSocketHandler();

        void Send_(std::string message) const;
        
    protected:
        virtual void Process_() override;
        void Transfer_();
        virtual void HandleNewConnection_(HTTP::Request& request, const WebSocketHandler& websocket_handler) = 0;
        virtual void HandleNewMessage_(const WebSocketHandler& websocket_handler, std::string message) = 0;
        virtual void HandleConnectionClosed_(const WebSocketHandler& websocket_handler) = 0;

    private:
		std::unique_ptr<WebSocket> websocket_;
};

#endif // NAF_HANDLERS_WEBSOCKETHANDLER
