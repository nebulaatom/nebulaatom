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

#ifndef ATOM_HANDLERS_WEBSOCKETHANDLER
#define ATOM_HANDLERS_WEBSOCKETHANDLER


#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"

#include "handlers/root_handler.h"


namespace Atom
{
    namespace Handlers
    {
        class WebSocketHandler;
    }
}


class Atom::Handlers::WebSocketHandler :
    public RootHandler
{
    public:
        WebSocketHandler();
        virtual ~WebSocketHandler();

        void Send_(std::string message) const;
        
    protected:
        virtual void AddFunctions_() override;
        virtual void Process_() override;
        virtual void HandleGETMethod_() override;
        virtual void HandlePOSTMethod_() override;
        virtual void HandlePUTMethod_() override;
        virtual void HandleDELMethod_() override;
        void Transfer_();
        virtual void HandleNewConnection_(HTTPServerRequestPtr request, const WebSocketHandler& websocket_handler) = 0;
        virtual void HandleNewMessage_(const WebSocketHandler& websocket_handler, std::string message) = 0;
        virtual void HandleConnectionClosed_(const WebSocketHandler& websocket_handler) = 0;

    private:
		std::unique_ptr<WebSocket> websocket_;
};

#endif // ATOM_HANDLERS_WEBSOCKETHANDLER
