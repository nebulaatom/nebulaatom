
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
            request.AddHeader_("cookie-example", "cookie-value");
            request.SetupResponseHeaders_();
            
            self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, "Hello!");
        });
    });

    return app.run(argc, argv);
}
