
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "http/common_responses.h"
#include "http/methods.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            HTTP::Methods methods;

            // GET method
            methods.AddProcess_(HTTP::HTTP_GET, [&](HTTP::Methods&)
            {
                self.HTMLResponse_(HTTP::Status::kHTTP_OK, "GET Method.");
            });

            // POST method
            methods.AddProcess_(HTTP::HTTP_POST, [&](HTTP::Methods&)
            {
                self.HTMLResponse_(HTTP::Status::kHTTP_OK, "POST Method.");
            });

            // Evaluate request method
            if(!methods.EvaluateProcess_(self.get_properties().method))
                self.HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "HTTP Method not found.");
        });
    });

    return app.Init_(argc, argv);
}
