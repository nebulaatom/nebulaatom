
#include "core/nebula_atom.h"
#include "functions/action.h"
#include "handlers/custom_handler.h"
#include "handlers/frontend_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"

using namespace NAF;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    Query::DatabaseManager::StartMySQL_();
    Tools::SettingsManager::ReadBasicProperties_();

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo& info)
    {
        Handlers::RootHandler* handler;

        // Manage the route type
        Tools::Route route(info.uri);

        switch(route.get_current_route_type())
        {
            // Manage Frontend
            case NAF::Tools::RouteType::kEntrypoint:
            {
                handler = new Handlers::FrontendHandler;
                break;
            }
            // Manage Backend
            case NAF::Tools::RouteType::kEndpoint:
            {
                handler = new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
                {
                    if(*self.get_requested_route() == Tools::Route("/api/products"))
                    {
                        Functions::Action a1("a1");
                        a1.set_sql_code("SELECT * FROM products");

                        a1.ComposeQuery_();
                        a1.ExecuteQuery_();
                        a1.MakeResults_();

                        auto result = a1.CreateJSONResult_();
                        result->set("status", a1.get_status());
                        result->set("message", a1.get_message());

                        self.CompoundResponse_(HTTP::Status::kHTTP_OK, result);
                    }
                    else
                        self.JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Endpoint not found.");
                });
                break;
            }
        }

        return handler;
    });

    // Run
        auto code = app.Init_(argc, argv);

    // End
        Query::DatabaseManager::StopMySQL_();
        return code;
}
