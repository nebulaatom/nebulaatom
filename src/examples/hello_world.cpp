
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

    // Setup
        Tools::SettingsManager::SetUpProperties_();

    auto hello_handler = new Core::HandlerFactory::FunctionRequest([&](const HTTPServerRequest& request)
    {
        return new HelloHandler;
    });
    auto& request_handler_creator = app.get_handler_factory()->get_request_handler_creator();
    request_handler_creator.reset(hello_handler);

    return app.run(argc, argv);
}
