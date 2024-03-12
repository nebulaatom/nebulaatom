
#include "core/nebula_atom.h"
#include "handlers/frontend_handler.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    // Setting up handler
        app.CustomHandlerCreator_([&](const HTTPServerRequest&)
        {
            return new Handlers::FrontendHandler;
        });

        return app.run(argc, argv);

}
