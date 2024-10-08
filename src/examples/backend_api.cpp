
#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "handlers/login_handler.h"
#include "http/methods.h"
#include "query/condition.h"
#include "query/parameter.h"
#include "query/results.h"
#include "tools/output_logger.h"
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
                auto f1 = AddFunction_("/api/products", HTTP::EnumMethods::kHTTP_GET);

                // Action 1
                    auto a1 = f1->AddAction_("a1");
                    a1->set_custom_error("No stores found with this name.");
                    a1->set_sql_code("SELECT id FROM stores WHERE name = ?");
                    a1->set_final(false);
                    a1->SetupCondition_("verify-identifier", Query::ConditionType::kWarning, [](Functions::Action& action)
                    {
                        if(action.get_results()->size() > 0)
                            Tools::OutputLogger::Debug_("Results > 0");
                        
                        return true;
                    });
                    // Parameters and conditions
                    auto param = a1->AddParameter_("storeName", Tools::DValue(""), true);
                    param->SetupCondition_("cond-param1", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                    {
                        if(param->ToString_() == "")
                        {
                            param->set_error("storeName cannot be iqual to a empty string");
                            return false;
                        }
                        else if(param->ToString_().size() < 3)
                        {
                            param->set_error("storeName cannot be less than 3 characters");
                            return false;
                        }
                        else
                            return true;
                    });

                // Action 2
                    auto a2 = f1->AddAction_("a2");
                    a2->set_custom_error("No products found.");
                    a2->set_sql_code("SELECT * FROM products WHERE id_store = ?");
                    a2->SetupCondition_("condition-action2", Query::ConditionType::kWarning, [](Functions::Action& action)
                    {
                        Tools::OutputLogger::Debug_("First: " + action.get_results()->First_()->ToString_());
                        Tools::OutputLogger::Debug_("Last: " + action.get_results()->Last_()->ToString_());
                        Tools::OutputLogger::Debug_("(2, 3): " + action.get_results()->ExtractField_(2, 3)->ToString_());
                        return true;
                    });

                    // Parameters and conditions
                    auto param2 = a2->AddParameter_("id_store", Query::Field::Position{0, 0}, "a1", false);
                    param2->SetupCondition_("cond-param2", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                    {
                        if(param->ToString_() == "")
                        {
                            param->set_error("id_store cannot be iqual to a empty string");
                            return false;
                        }
                        else if(param->get_value().get_type() != Tools::DValue::Type::kInteger)
                        {
                            param->set_error("id_store must be an integer value");
                            return false;
                        }
                        if(param->IntValue_() < 0)
                        {
                            param->set_error("id_store must be greather or iqual than 0");
                            return false;
                        }
                        else
                            return true;
                    });
            
            // Function /api/image
                auto f2 = AddFunction_("/api/image", HTTP::EnumMethods::kHTTP_GET);
                f2->set_response_type(Functions::Function::ResponseType::kFile);

                f2->get_file_manager()->set_directory_base("/var/www/uploaded-files");
                f2->get_file_manager()->AddBasicSupportedFiles_();

                // Action 1
                auto action = f2->AddAction_("a1");
                action->set_custom_error("No image found.");
                action->set_sql_code("SELECT file_path FROM test_filepaths WHERE id = ?");
                action->AddParameter_("id", Tools::DValue(0), true);
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
                    JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint ("+get_properties().method+") is not available.");
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
        Tools::SettingsManager::ReadSettings_();
        app.SetupSettings_();
        Query::DatabaseManager::StartMySQL_();
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
