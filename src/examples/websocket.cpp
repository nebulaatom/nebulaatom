
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "handlers/websocket_handler.h"

using namespace Atom;



class MainHandler : public Handlers::WebSocketHandler
{
    public:
        MainHandler(std::vector<const WebSocketHandler*>& connected_sockets) : connected_sockets_(connected_sockets){}
        virtual ~MainHandler(){}

        void HandleNewConnection_(HTTP::Request&, const WebSocketHandler& websocket_handler) override
        {
            connected_sockets_.push_back(&websocket_handler);
        }
        void HandleNewMessage_(const WebSocketHandler&, std::string message) override
        {
            for(auto it : connected_sockets_)
                it->Send_(message);
        }
        void HandleConnectionClosed_(const WebSocketHandler& websocket_handler) override
        {
            for (auto it = connected_sockets_.begin() ; it != connected_sockets_.end(); ++it)
            {
                if (*it == &websocket_handler)
                {
                    connected_sockets_.erase(it);
                    break;
                }
            }
        }

    private:
        std::vector<const Handlers::WebSocketHandler*>& connected_sockets_;
};


int main(int argc, char** argv)
{
    Core::NebulaAtom app;
    
    std::vector<const Handlers::WebSocketHandler*> connected_sockets;
    app.CustomHandlerCreator_([&](HTTP::Request& request)
    {
        Handlers::RootHandler* handler;
        auto& http_request = *request.get_http_server_const_request().value();

		if(http_request.find("Upgrade") != http_request.end() && Poco::icompare(http_request["Upgrade"], "websocket") == 0)
        {
            handler = new MainHandler(connected_sockets);
        }
		else
        {
            handler = new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
            {
                std::string html = "<html><head><title>WebSocketServer</title>";
                html += "</head>";
                html += "<body>";
                html += "<textarea id=\"message\"></textarea>";
                html += "<button id=\"send\">Send message</button>";
                html += "<ul id=\"messageList\"></ul>";
                html += "<script type=\"text/javascript\">";
                html += "   const socket = new WebSocket('ws://" + http_request.serverAddress().toString() + "/ws');";
                html += "   socket.onmessage = function(event)";
                html += "   { ";
                html += "      const message = event.data;";
                html += "      const messageList = document.getElementById('messageList');";
                html += "      const listItem = document.createElement('li');";
                html += "      listItem.textContent = message;";
                html += "      messageList.appendChild(listItem);";
                html += "   };";
                html += "   socket.onclose = function()";
                html += "   { ";
                html += "      alert(\"WebSocket closed.\");";
                html += "   };";
                html += "   document.querySelector('#send').onclick = function(e)";
                html += "   { ";
                html += "      socket.send(document.querySelector('#message').value);";
                html += "      document.querySelector('#message').value = '';";
                html += "   };";
                html += "</script>";
                html += "</body>";
                html += "</html>";

                self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, html);
            });
        }

        return handler;
    });

    return app.run(argc, argv);
}
