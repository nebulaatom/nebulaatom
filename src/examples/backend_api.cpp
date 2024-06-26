
#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "handlers/login_handler.h"
#include "http/methods.h"
#include "query/condition.h"
#include "query/results.h"
#include "tools/output_logger.h"
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
                auto f1 = AddFunction_("/api/products", HTTP::EnumMethods::kHTTP_GET);

                // Action 1
                    auto a1 = f1->AddAction_("a1");
                    a1->set_custom_error("No stores found with this name.");
                    a1->set_sql_code("SELECT id FROM stores WHERE name = ?");
                    // Parameters
                        a1->AddParameter_("storeName", Tools::DValue(""), true);
                    // Conditions
                        a1->AddCondition_("condition1", Query::ConditionType::kError, [](Query::Results::Ptr results)
                        {
                            if(results->size() == 0) return false;
                            else return true;
                        });

                // Action 2
                    auto a2 = f1->AddAction_("a2");
                    a2->set_custom_error("No products found.");
                    a2->set_sql_code("SELECT * FROM products WHERE id_store = ?");
                    a2->set_final(true);
                    // Parameters
                        a2->AddParameter_("id_store", Query::Field::Position{0, 0}, "a1", false);
                    // Conditions
                        a2->AddCondition_("condition1", Query::ConditionType::kError, [](Query::Results::Ptr results)
                        {
                            // Example of how to scroll through the results
                            for(auto row : *results)
                            {
                                std::string string_row = "";
                                for(auto field : *row)
                                    string_row += field->get_column_name() + ": " + field->get_value().ToString_() + ", ";

                                Tools::OutputLogger::Log_(string_row);
                            }
                            
                            return true;
                        });

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
                    JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint ("+get_method()+") is not available.");
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
