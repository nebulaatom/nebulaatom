
#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "handlers/login_handler.h"
#include "http/methods.h"
#include "query/condition.h"
#include "query/results.h"
#include "tools/route.h"
#include "tools/dvalue.h"

using namespace NAF;

class MainHandler : public Handlers::BackendHandler
{
    public:
        MainHandler() : Handlers::BackendHandler(){}
        virtual ~MainHandler() {}

        void AddFunctions_()
        {
            // Function /api/products
                auto f1 = AddFunction_("/api/products", HTTP::EnumMethods::kHTTP_POST);

                // Action
                    auto action = f1->AddAction_("a1");
                    action->set_custom_error("Error to add product.");
                    action->set_sql_code("INSERT INTO products (name, price, id_store) VALUES (?, ?, ?)");
                    // Parameters and conditions
                    action->AddParameter_("productName", Tools::DValue(""), true)
                    ->SetupCondition_("condition-productName", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                    {
                        if(param->ToString_() == "")
                        {
                            param->set_error("productName cannot be iqual to a empty string");
                            return false;
                        }
                        else
                            return true;
                    });
                    action->AddParameter_("productPrice", Tools::DValue(""), true)
                    ->SetupCondition_("condition-productPrice", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                    {
                        if(param->ToString_() == "")
                        {
                            param->set_error("productPrice cannot be iqual to a empty string");
                            return false;
                        }
                        else
                            return true;
                    });
                    action->AddParameter_("idStore", Tools::DValue(""), true)
                    ->SetupCondition_("condition-idStore", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                    {
                        if(param->ToString_() == "")
                        {
                            param->set_error("idStore cannot be iqual to a empty string");
                            return false;
                        }
                        else
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
                    JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint (" + get_properties().method + ") is not available.");
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
            NAF::Tools::Route requested_route(info.uri);
            NAF::Tools::Route login_route({"api", "system", "login"});
            NAF::Tools::Route logout_route({"api", "system", "logout"});

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
