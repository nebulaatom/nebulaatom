
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "http/request.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    app.CustomHandlerCreator_([&](HTTP::Request& request)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            // Set header
            request.AddHeader_("cookie-example", "cookie-value");
            request.SetupResponseHeaders_();

            // View headers
            for(auto& header : *request.get_http_server_request().value())
                std::cout << header.first << ": " << header.second << std::endl;
            
            self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, "Hello!");
        });
    });

    return app.Init_(argc, argv);
}
