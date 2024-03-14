
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    app.CustomHandlerCreator_([&](const HTTPServerRequest&, Core::HandlerFactory&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.CustomHTMLResponse_(HTTP::Status::kOK, "Hello!");
        });
    });

    return app.run(argc, argv);
}
