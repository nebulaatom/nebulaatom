
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
#include "handlers/backend_handler.h"
#include "handlers/frontend_handler.h"
#include "handlers/login_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"
#include "tools/settings_manager.h"
#include <Poco/JSON/Handler.h>

using namespace CPW;

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
