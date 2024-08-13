
#include "core/nebula_atom.h"
#include "functions/action.h"
#include "handlers/custom_handler.h"
#include "http/body.h"
#include "http/request.h"
#include "tools/output_logger.h"
#include <Poco/JSON/Object.h>

using namespace NAF;

void ViewParameters(Handlers::CustomHandler& self)
{
    switch(self.get_body_type())
    {
        case HTTP::Body::Type::kFormMultipart:
        {
            for(auto& file : self.get_files_parameters()->get_files())
                Tools::OutputLogger::Debug_("FormMultipart parameter: " + file.get_name() + ", " + file.get_filename());
            for(auto& value : *self.get_form())
                Tools::OutputLogger::Debug_("FormMultipart parameter: " + value.first + ", " + value.second);
            break;
        }
        case HTTP::Body::Type::kJSON:
        {
            self.get_json_array()->stringify(std::cout);
            self.get_json_object()->stringify(std::cout);
            break;
        }
        case HTTP::Body::Type::kFormURLEncoded:
        case HTTP::Body::Type::kURI:
        {
            for(auto param : self.get_query_parameters())
                Tools::OutputLogger::Debug_("URI parameter: " + param.first + ", " + param.second);
            self.get_json_array()->stringify(std::cout);
            break;
        }
    }
}

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    Tools::SettingsManager::ReadSettings_();
    app.SetupSettings_();
    Query::DatabaseManager::StartMySQL_();

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            // Process the request body
            self.ManageRequestBody_();

            // View parameters
            ViewParameters(self);

            // Create query
            Functions::Action a1("a1");
            a1.set_sql_code("SELECT * FROM products");

            a1.ComposeQuery_();
            a1.ExecuteQuery_();
            a1.MakeResults_();

            auto result = a1.CreateJSONResult_();
            result->set("status", a1.get_status());
            result->set("message", a1.get_message());

            self.CompoundResponse_(HTTP::Status::kHTTP_OK, result);

            // Create ASYNC query (for logs, notifications and another activities)

            Functions::Action a2("a2");
            a1.set_sql_code("INSERT INTO test_logs (log) VALUES ('test log')");
            a1.ComposeQuery_();
            a1.ExecuteAsyncQuery_();
            Tools::OutputLogger::Log_("Continue...");
        });
    });

    // Run
        auto code = app.Init_(argc, argv);

    // End
        Query::DatabaseManager::StopMySQL_();
        return code;
}
