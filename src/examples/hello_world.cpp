
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
#include "handlers/custom_handler.h"
#include <Poco/Net/HTTPResponse.h>

using namespace CPW;

int main(int argc, char** argv)
{
    Core::WoodpeckerServer app;

    app.CustomHandlerCreator_([&](const HTTPServerRequest&)
    {
        auto handler = new Handlers::CustomHandler;
        handler->SetProcessFunction([&]()
        {
            handler->CustomHTMLResponse_(*handler->get_response(), HTTPResponse::HTTP_OK, "Hello!");
        });
        return handler;
    });

    return app.run(argc, argv);
}
