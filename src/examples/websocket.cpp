
#include "core/nebula_atom.h"
#include "handlers/null_handler.h"
#include "handlers/custom_handler.h"
#include "handlers/websocket_handler.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    app.CustomHandlerCreator_([&](const HTTPServerRequest& request, Core::HandlerFactory& handler_factory)
    {
        Handlers::RootHandler* handler;

		if(request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0)
        {
            handler = new Handlers::WebSocketHandler(handler_factory.get_connected_sockets());
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
                html += "   const socket = new WebSocket('ws://" + request.serverAddress().toString() + "/ws');";
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

                self.CustomHTMLResponse_(HTTP::Status::kOK, html);
            });
        }

        return handler;
    });

    return app.run(argc, argv);
}
