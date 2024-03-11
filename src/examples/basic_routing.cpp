
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
#include "handlers/custom_handler.h"
#include "http/common_responses.h"

using namespace CPW;

int main(int argc, char** argv)
{
    Core::WoodpeckerServer app;
    
    app.AddHandler_("/json", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& handler)
        {
            handler.JSONResponse_(HTTP::Status::kOK, "Hello!");
        });
    });
    app.AddHandler_("/html", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& handler)
        {
            handler.HTMLResponse_(HTTP::Status::kOK, "Hello!");
        });
    });

    return app.run(argc, argv);
}
