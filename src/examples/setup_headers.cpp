
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "http/request.h"

using namespace NAF;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            // Set header
            self.AddHeader_("header-example", "header-value");
            self.AddCookie_("cookie-example", "cookie-value");

            // View headers
            auto headers = self.get_http_server_request().value();
            for(auto& header : *headers)
                std::cout << header.first << ": " << header.second << std::endl;
            
            self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, "Hello!");
        });
    });

    return app.Init_(argc, argv);
}
