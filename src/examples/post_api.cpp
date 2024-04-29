
#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "handlers/login_handler.h"
#include "http/methods.h"
#include "query/condition.h"
#include "query/results.h"
#include "tools/route.h"
#include "tools/dvalue.h"

using namespace Atom;

class MainHandler : public Handlers::BackendHandler
{
    public:
        MainHandler() : Handlers::BackendHandler(){}
        virtual ~MainHandler() {}

        void AddFunctions_()
        {
            // Function /api/products
                auto f1 = AddFunction_("/api/products", HTTP::EnumMethods::kHTTP_POST);

                // Action 1
                    auto a1 = f1->AddSQLAction_("a1");
                    a1->set_custom_error("The product exists.");
                    a1->set_sql_code("SELECT * FROM products WHERE name = ?");
                    // Parameters
                        a1->AddParameter_("productName", Tools::DValue(""), true);
                    // Conditions
                        a1->AddCondition_("condition1", Query::ConditionType::kError, [](Query::Results::Ptr results)
                        {
                            if(results->size() > 0)
                                return false;
                            else
                                return true;
                        });

                // Action 2
                    auto a2 = f1->AddSQLAction_("a2");
                    a2->set_custom_error("Error to add product.");
                    a2->set_sql_code("INSERT INTO products (name, price, id_store) VALUES (?, ?, ?)");
                    a2->set_final(true);
                    // Parameters
                        a2->AddParameter_("productName", Tools::DValue(""), true);
                        a2->AddParameter_("productPrice", Tools::DValue(""), true);
                        a2->AddParameter_("idStore", Tools::DValue(""), true);
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
            // Routes
            Atom::Tools::Route requested_route(info.uri);
            Atom::Tools::Route login_route({"api", "system", "login"});
            Atom::Tools::Route logout_route({"api", "system", "logout"});

            // Return handler
            Handlers::RootHandler* handler;
            if(requested_route == login_route || requested_route == logout_route)
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
