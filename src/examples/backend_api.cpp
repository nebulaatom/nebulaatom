
#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "handlers/login_handler.h"
#include "http/methods.h"
#include "tools/route.h"

using namespace Atom;

class MainHandler : public Handlers::BackendHandler
{
    public:
        MainHandler() : Handlers::BackendHandler(){}
        virtual ~MainHandler() {}

        void AddFunctions_()
        {
            // Function /api/products
                Functions::Function::Ptr f1(new Functions::Function{"/api/products", HTTP::EnumMethods::kHTTP_GET});

                // Action 1
                    auto a1 = std::make_shared<Functions::SQLAction>("a1");
                    a1->set_custom_error("No stores found with this name.");
                    a1->set_sql_code("SELECT id FROM stores WHERE name = ?");
                    // Parameters
                        a1->get_parameters().push_back(Query::Parameter{"storeName", Tools::RowValueFormatter{std::string("")}, true});
                    f1->get_actions().push_back(a1);

                // Action 2
                    auto a2 = std::make_shared<Functions::SQLAction>("a2");
                    a2->set_custom_error("No products found.");
                    a2->set_sql_code("SELECT * FROM products WHERE id_store = ?");
                    a2->set_final(true);
                    // Parameters
                        a2->get_parameters().push_back(Query::Parameter{"id_store", Query::ConditionalField{0, 0}, "a1", false});
                    f1->get_actions().push_back(a2);

            // Setting up the function
                    get_functions_manager().get_functions().insert({f1->get_endpoint(), f1});

        }

        void Process_() override
        {
            AddFunctions_();

            // Set security type
                set_security_type(Extras::SecurityType::kDisableAll);
                
            // Process the request body
                ManageRequestBody_();

            // Route identification
                if(!IdentifyRoute_())
                {
                    JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint is not available.");
                    return;
                }

                if(!VerifySession_())
                {
                    JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Session not found.");
                    return;
                }

                if(!VerifyPermissions_())
                {
                    JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "The user does not have the permissions to perform this operation.");
                    return;
                }

            // Process actions
                ProcessActions_();
        }
};

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    // Setup
        Query::DatabaseManager::StartMySQL_();
        Tools::SettingsManager::ReadBasicProperties_();
        Security::PermissionsManager::LoadPermissions_();

    // Read sessions
        Tools::SessionsManager::ReadSessions_();

    // Setting up handler
        app.CustomHandlerCreator_([&](Core::HTTPRequestInfo& info)
        {
            // Set route
            std::vector<std::string> segments;
            URI(info.uri).getPathSegments(segments);
            Atom::Tools::Route requested_route(segments);

            // Login/logout routes
            std::vector<std::string> login_route({"api", "system", "login"});
            std::vector<std::string> logout_route({"api", "system", "logout"});

            // Return handler
            Handlers::RootHandler* handler;
            if(requested_route.get_segments() == login_route || requested_route.get_segments() == logout_route)
                handler = new Handlers::LoginHandler();
            else
                handler = new MainHandler;

            return handler;
        });

    // Run
        auto code = app.Init_(argc, argv);

    // End
        Query::DatabaseManager::StopMySQL_();
        return code;
}
