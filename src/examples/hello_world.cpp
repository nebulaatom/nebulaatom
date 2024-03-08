
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
#include "handlers/root_handler.h"
#include <Poco/Net/HTTPResponse.h>

using namespace CPW;

class HelloHandler : public Handlers::RootHandler
{
    public:
        HelloHandler() : Handlers::RootHandler(){}
        virtual ~HelloHandler() {}

        void Process_() override
        {
            // Say hello
            HTMLResponse_(*get_response(), HTTPResponse::HTTP_OK, "Hello!");
        }
};

int main(int argc, char** argv)
{
    Core::WoodpeckerServer app;

    app.CustomHandlerCreator_([&](const HTTPServerRequest&)
    {
        return new HelloHandler;
    });

    return app.run(argc, argv);
}
