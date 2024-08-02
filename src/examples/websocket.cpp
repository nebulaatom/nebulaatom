
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "handlers/websocket_handler.h"

using namespace NAF;



class MainHandler : public Handlers::WebSocketHandler
{
    public:
        MainHandler(VectorSocket& connected_sockets) : connected_sockets_(connected_sockets){}
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
        VectorSocket& connected_sockets_;
};


int main(int argc, char** argv)
{
    Core::NebulaAtom app;
    
    Handlers::WebSocketHandler::VectorSocket connected_sockets;
    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo& info)
    {
        Handlers::RootHandler* handler;

		if(info.uri == "/ws")
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
                html += "   const socket = new WebSocket('ws://" + self.get_http_server_request().value()->serverAddress().toString() + "/ws');";
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

    return app.Init_(argc, argv);
}
