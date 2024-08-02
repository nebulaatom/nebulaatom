
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"

using namespace NAF;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;
    
    app.AddHandler_("/json", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& handler)
        {
            handler.JSONResponse_(HTTP::Status::kHTTP_OK, "Hello!");
        });
    });
    app.AddHandler_("/html", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& handler)
        {
            handler.HTMLResponse_(HTTP::Status::kHTTP_OK, "Hello!");
        });
    });

    return app.Init_(argc, argv);
}
