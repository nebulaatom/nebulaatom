
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
#include "handlers/null_handler.h"
#include "handlers/root_handler.h"

using namespace CPW;

class HTMLHandler : public Handlers::RootHandler
{
    public:
        HTMLHandler() : Handlers::RootHandler(){}
        virtual ~HTMLHandler() {}

        void Process_() override
        {
            // Say Hello
            HTMLResponse_(*get_response(), HTTPResponse::HTTP_OK, "Hello!");
        }
};

class JSONHandler : public Handlers::RootHandler
{
    public:
        JSONHandler() : Handlers::RootHandler(){}
        virtual ~JSONHandler() {}

        void Process_() override
        {
            // Say Hello
            GenericResponse_(*get_response(), HTTPResponse::HTTP_OK, "Hello!");
        }
};

int main(int argc, char** argv)
{
    Core::WoodpeckerServer app;
    
    // Setup
        Tools::SettingsManager::SetUpProperties_();

    auto hello_handler = new Core::HandlerFactory::FunctionRequest([&](const HTTPServerRequest& request)
    {
        CPW::Handlers::RootHandler* handler = nullptr;
        if(request.getURI() == "/json")
        {
            handler = new JSONHandler;
        }
        else if(request.getURI() == "/html")
        {
            handler = new HTMLHandler;
        }
        else
        {
            handler = new Handlers::NullHandler();
        }

        return handler;
    });

    auto& request_handler_creator = app.get_handler_factory()->get_request_handler_creator();
    request_handler_creator.reset(hello_handler);

    return app.run(argc, argv);
}
