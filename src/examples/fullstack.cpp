
#include "core/nebula_atom.h"
#include "functions/sql_action.h"
#include "handlers/custom_handler.h"
#include "handlers/frontend_handler.h"
#include "handlers/root_handler.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    Query::DatabaseManager::StartMySQL_();
    Tools::SettingsManager::ReadBasicProperties_();

    app.CustomHandlerCreator_([&](const HTTPServerRequest& request)
    {
        Handlers::RootHandler* handler;

        // Manage the route type
        std::vector<std::string> segments;
        URI(request.getURI()).getPathSegments(segments);
        auto route = std::make_shared<Tools::Route>(segments);

        switch(route->get_current_route_type())
        {
            // Manage Frontend
            case Atom::Tools::RouteType::kEntrypoint:
            {
                handler = new Handlers::FrontendHandler;
                break;
            }
            // Manage Backend
            case Atom::Tools::RouteType::kEndpoint:
            {
                handler = new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
                {
                    if(route->SegmentsToString_() == "/api/products")
                    {
                        Functions::SQLAction a1("a1");
                        a1.set_sql_code("SELECT * FROM products");

                        a1.ComposeQuery_();
                        a1.ExecuteQuery_();
                        a1.MakeResults_();

                        a1.get_json_result() = a1.CreateJSONResult_();
                        a1.get_json_result()->set("status", a1.get_status());
                        a1.get_json_result()->set("message", a1.get_message());

                        self.CompoundResponse_(HTTP::Status::kHTTP_OK, a1.get_json_result());
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
        auto code = app.run(argc, argv);

    // End
        Query::DatabaseManager::StopMySQL_();
        return code;
}
