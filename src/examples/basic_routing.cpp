
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
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
    
    app.AddHandler_("/json", [&](){ return new JSONHandler; });
    app.AddHandler_("/html", [&](){ return new HTMLHandler; });

    return app.run(argc, argv);
}
