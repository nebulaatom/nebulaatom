
#include "core/handler_factory.h"
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "http/common_responses.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo& info)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            if(info.uri == "/hello")
                self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, "Hello! Custom error");
            else
                self.FileResponse_(HTTP::Status::kHTTP_NOT_FOUND, "/var/www/404.html");
        });
    });

    return app.Init_(argc, argv);
}
