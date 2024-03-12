
#include "core/woodpecker_server.h"
#include "handlers/custom_handler.h"

using namespace CPW;

int main(int argc, char** argv)
{
    Core::WoodpeckerServer app;

    app.CustomHandlerCreator_([&](const HTTPServerRequest&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.CustomHTMLResponse_(HTTP::Status::kOK, "Hello!");
        });
    });

    return app.run(argc, argv);
}
