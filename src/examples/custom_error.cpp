
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "http/common_responses.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    app.CustomHandlerCreator_([&](HTTP::Request& request)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            if(request.get_uri() == "/hello")
                self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, "Hello! Custom error");
            else
                self.FileResponse_(HTTP::Status::kHTTP_NOT_FOUND, "/var/www/404.html");
        });
    });

    return app.run(argc, argv);
}
