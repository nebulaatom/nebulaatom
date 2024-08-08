
#include "core/nebula_atom.h"
#include "functions/action.h"
#include "handlers/custom_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"

using namespace NAF;

class Frontend : public Handlers::RootHandler
{
    public:
        Frontend() : RootHandler(){}

        void Process_()
        {
            file_manager_.AddBasicSupportedFiles_();
            auto method = GetMethod_(get_properties().method);
            switch(method)
            {
                case HTTP::EnumMethods::kHTTP_GET:
                    DownloadProcess_();
                    break;
                case HTTP::EnumMethods::kHTTP_POST:
                case HTTP::EnumMethods::kHTTP_PUT:
                case HTTP::EnumMethods::kHTTP_DEL:
                case HTTP::EnumMethods::kHTTP_HEAD:
                case HTTP::EnumMethods::kHTTP_OPTIONS:
                case HTTP::EnumMethods::kHTTP_PATCH:
                case HTTP::EnumMethods::kNULL:
                    JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
                    break;
            }
        }

        void DownloadProcess_()
        {
            // Manage the file

                auto& request = get_http_server_request().value();
                file_manager_.set_operation_type(Files::OperationType::kDownload);
                file_manager_.get_files().push_back(file_manager_.CreateTempFile_(request->getURI()));
                auto& tmp_file = file_manager_.get_files().front();

            // Check file
                if(!file_manager_.CheckFiles_())
                {
                    HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
                    return;
                }

            // Is supported
                if(!file_manager_.IsSupported_(tmp_file))
                {
                    HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
                    return;
                }
                file_manager_.ProcessContentLength_();

            // Reponse
                auto& response = get_http_server_response().value();
                response->setStatus(HTTPResponse::HTTP_OK);
                response->setContentType(tmp_file.get_content_type());
                response->setContentLength(tmp_file.get_content_length());
                
                std::ostream& out_reponse = response->send();

            // Download file
                file_manager_.DownloadFile_(out_reponse);
        }

    private:
        Files::FileManager file_manager_;
};

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
                handler = new Frontend;
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
