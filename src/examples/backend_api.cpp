
#include "core/handler_factory.h"
#include "core/woodpecker_server.h"
#include "handlers/backend_handler.h"
#include "handlers/login_handler.h"
#include "handlers/null_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"
#include "tools/settings_manager.h"

using namespace CPW;

class MainHandler : public Handlers::BackendHandler
{
    public:
        MainHandler() : Handlers::BackendHandler("v1"){}
        virtual ~MainHandler() {}

        void AddActions_() override
        {
            // Function /api/products
                std::string endpoint = "/api/products";
                Functions::Function f1{endpoint, Functions::Function::Type::kGET};

                // Setting up the actions
                    auto a1 = std::make_shared<Functions::SQLAction>("a1");
                    a1->set_custom_error("No stores found with this name.");
                    a1->set_sql_code("SELECT id FROM stores WHERE name = ?");
                    // Parameters
                        a1->get_parameters().push_back(Query::Parameter{"storeName", Tools::RowValueFormatter{std::string("")}, true});
                    f1.get_actions().push_back(a1);

                // Setting up the action
                    auto a2 = std::make_shared<Functions::SQLAction>("a2");
                    a2->set_custom_error("No products found.");
                    a2->set_sql_code("SELECT * FROM products WHERE id_store = ?");
                    a2->set_final(true);
                    // Parameters
                        a2->get_parameters().push_back(Query::Parameter{"id_store", Query::ConditionalField{0, 0}, "a1", false});
                    f1.get_actions().push_back(a2);

                // Setting up the function
                    get_functions_manager().get_functions().insert({endpoint, std::move(f1)});
                    get_routes_list().push_back(Tools::Route("api/products"));

        }

        void Process_() override
        {
            try
            {
                // Prepare routes
                    AddActions_();

                    std::vector<std::string> segments;
                    URI(get_request()->getURI()).getPathSegments(segments);
                    get_requested_route() = std::make_shared<Tools::Route>(segments);

                // Set security type
                    get_current_security().set_security_type(Extras::SecurityType::kDisableAll);
                    
                // Route identification
                    if(!IdentifyRoute_())
                    {
                        GenericResponse_(*get_response(), HTTPResponse::HTTP_NOT_FOUND, "The requested endpoint is not available.");
                        return;
                    }

                    if(!VerifySession_())
                    {
                        GenericResponse_(*get_response(), HTTPResponse::HTTP_UNAUTHORIZED, "Session not found.");
                        return;
                    }

                    if(!VerifyPermissions_())
                        return;

                // Process actions
                    ProcessActions_();

            }
            catch(std::exception& error)
            {
                app_.logger().error("- Error on backend_api.cpp on Process_(): " + std::string(error.what()));
                GenericResponse_(*get_response(), HTTPResponse::HTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
            }
            catch(...)
            {
                app_.logger().error("- Error on backend_api.cpp on Process_(): No handled exception.");
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
            Query::DatabaseManager::StartMySQL_();
            Tools::SettingsManager::SetUpProperties_();
            Tools::SettingsManager::ReadBasicProperties_();
            Security::PermissionsManager::LoadPermissions_();

        // Read sessions
            Tools::SessionsHandler::ReadSessions_();

        // Setting up handler
            auto hello_handler = new Core::HandlerFactory::FunctionRequest([&](const HTTPServerRequest& request)
            {
                // Set route
                std::vector<std::string> segments;
                URI(request.getURI()).getPathSegments(segments);
                CPW::Tools::Route requested_route(segments);

                // Login/logout routes
                std::vector<std::string> login_route({"api", "system", "login"});
                std::vector<std::string> logout_route({"api", "system", "logout"});

                // Return handler
                Handlers::RootHandler* handler;
                if(requested_route.get_segments() == login_route || requested_route.get_segments() == logout_route)
                    handler = new Handlers::LoginHandler("v1");
                else
                    handler = new MainHandler;

                return handler;
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

    Query::DatabaseManager::StopMySQL_();
}
