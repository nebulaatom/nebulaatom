
#include "core/woodpecker_server.h"
#include "handlers/frontend_handler.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::WoodpeckerServer app;

    // Setting up handler
        app.CustomHandlerCreator_([&](const HTTPServerRequest&)
        {
            return new Handlers::FrontendHandler;
        });

        return app.run(argc, argv);

}
