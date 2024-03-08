
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
#include "handlers/custom_handler.h"

using namespace CPW;

int main(int argc, char** argv)
{
    Core::WoodpeckerServer app;
    
    app.AddHandler_("/json", [&]()
    {
        auto handler = new Handlers::CustomHandler;
        handler->SetProcessFunction([&]()
        {
            // Say hello in JSON
            handler->GenericResponse_(*handler->get_response(), HTTPResponse::HTTP_OK, "Hello!");
        });
        return handler;
    });
    app.AddHandler_("/html", [&]()
    {
        auto handler = new Handlers::CustomHandler;
        handler->SetProcessFunction([&]()
        {
            // Say hello in HMTL
            handler->HTMLResponse_(*handler->get_response(), HTTPResponse::HTTP_OK, "Hello!");
        });
        return handler;
    });

    return app.run(argc, argv);
}
