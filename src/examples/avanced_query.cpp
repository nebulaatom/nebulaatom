
#include "core/nebula_atom.h"
#include "functions/sql_action.h"
#include "handlers/custom_handler.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    Query::DatabaseManager::StartMySQL_();
    Tools::SettingsManager::ReadBasicProperties_();

    app.CustomHandlerCreator_([&](const HTTPServerRequest&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            Functions::SQLAction a1("a1");
            a1.set_sql_code("SELECT * FROM products");

            a1.ComposeQuery_();
            a1.ExecuteQuery_();
            a1.MakeResults_();

            a1.get_json_result() = a1.CreateJSONResult_();
            a1.get_json_result()->set("status", a1.get_status());
            a1.get_json_result()->set("message", a1.get_message());

            self.CompoundResponse_(HTTP::Status::kOK, a1.get_json_result());
        });
    });

    // Run
        auto code = app.run(argc, argv);

    // End
        Query::DatabaseManager::StopMySQL_();
        return code;
}
