
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
#include "handlers/root_handler.h"
#include <Poco/Net/HTTPResponse.h>

using namespace CPW;

class HelloHandler : public Handlers::RootHandler
{
    public:
        HelloHandler() : Handlers::RootHandler("v1"){}
        virtual ~HelloHandler() {}

        void Process_() override
        {
            try
            {
                // Say Hello
                    HTMLResponse_(*get_response(), HTTPResponse::HTTP_OK, "Hello!");
            }
            catch(std::exception& error)
            {
                app_.logger().error("- Error on hello_world.cpp on Process_(): " + std::string(error.what()));
                GenericResponse_(*get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
            }
            catch(...)
            {
                app_.logger().error("- Error on hello_world.cpp on Process_(): No handled exception.");
                GenericResponse_(*get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. No handled exception." );
            }
        }
};

int main(int argc, char** argv)
{
	try
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
    catch (std::exception const& error)
    {
        Application::instance().logger().error("- Error on hello_world.cpp on main(): " + std::string(error.what()));
        return -1;
    }
    catch (...)
    {
        Application::instance().logger().error("- Error on hello_world.cpp on main(): uncaught error");
        return -1;
    }
}
