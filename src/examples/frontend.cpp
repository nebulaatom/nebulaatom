
#include "core/nebula_atom.h"
#include "functions/action.h"
#include "functions/function.h"
#include "handlers/custom_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"
#include "files/file_manager.h"

using namespace NAF;
using namespace NAF::Handlers;

class MainHandler : public Handlers::BackendHandler
{
    public:
        MainHandler() : Handlers::BackendHandler(){}
        virtual ~MainHandler() {}

        void AddFunctions_()
        {
            // Function /api/file/read
                auto f1 = AddFunction_("/api/file/read", HTTP::EnumMethods::kHTTP_GET);
                f1->set_response_type(Functions::Function::ResponseType::kFile);

                f1->get_file_manager()->set_directory_base("/var/www/uploaded-files");
                f1->get_file_manager()->AddBasicSupportedFiles_();

                // Action 1
                auto a1 = f1->AddAction_("");
                a1->set_custom_error("No image found.");
                a1->set_sql_code("SELECT '2024811_18502548_1611731744_aire_acondicionado_tamaños.png'");

            // Function /api/file/add
                auto f2 = AddFunction_("/api/file/add", HTTP::EnumMethods::kHTTP_POST);
                f2->set_response_type(Functions::Function::ResponseType::kFile);

                f2->get_file_manager()->set_directory_base("/var/www/uploaded-files");
                f2->get_file_manager()->AddSupportedFile_("png", Files::FileProperties{"image/png", true, {""}});

            // Function /api/file/modify
                auto f3 = AddFunction_("/api/file/modify", HTTP::EnumMethods::kHTTP_PUT);
                f3->set_response_type(Functions::Function::ResponseType::kFile);

                f3->get_file_manager()->set_directory_base("/var/www/uploaded-files");
                f3->get_file_manager()->AddBasicSupportedFiles_();

                // Action 1
                auto a3 = f3->AddAction_("");
                a3->set_custom_error("No image found.");
                a3->set_sql_code("SELECT '20160315_v210.png'");

            // Function /api/file/remove
                auto f4 = AddFunction_("/api/file/remove", HTTP::EnumMethods::kHTTP_DEL);
                f4->set_response_type(Functions::Function::ResponseType::kFile);

                f4->get_file_manager()->set_directory_base("/var/www/uploaded-files");
                f4->get_file_manager()->AddBasicSupportedFiles_();

                // Action 1
                auto a4 = f4->AddAction_("");
                a4->set_custom_error("No image found.");
                a4->set_sql_code("SELECT '2024811_18502548_1611731744_aire_acondicionado_tamaños.png'");
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

            // Process actions
                ProcessActions_();
        }
};

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    Query::DatabaseManager::StartMySQL_();
    Tools::SettingsManager::ReadBasicProperties_();

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo& info)
    {
            // Routes
            NAF::Tools::Route requested_route(info.uri);
            NAF::Tools::Route upload("/upload");
            NAF::Tools::Route modify("/modify");

            // Return handler
            Handlers::RootHandler* handler;
            if(requested_route == upload)
                handler = new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
                {
                    self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, std::string(" \
                        <html><head><title>Upload file example</title></head> \
                        <body><input type=\"file\" id=\"image-file\"><button onclick=\"upload()\">Upload</button><script> \
                            function upload(){ \
                                let photo = document.getElementById(\"image-file\").files[0]; \
                                let formData = new FormData(); \
                                formData.append(\"photo\", photo); \
                                fetch('/api/file/add', {method: \"POST\", body: formData}); \
                            } \
                        </script></body> \
                    "));
                });
            else if(requested_route == modify)
                handler = new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
                {
                    self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, std::string(" \
                        <html><head><title>Modify file example</title></head> \
                        <body><input type=\"file\" id=\"image-file\"><button onclick=\"modify()\">Upload</button><script> \
                            function modify(){ \
                                let photo = document.getElementById(\"image-file\").files[0]; \
                                let formData = new FormData(); \
                                formData.append(\"photo\", photo); \
                                fetch('/api/file/modify', {method: \"PUT\", body: formData}); \
                            } \
                        </script></body> \
                    "));
                });
            else
                handler = new MainHandler;

            return handler;
    });

    return app.Init_(argc, argv);
}
