
#include "core/nebula_atom.h"
#include "files/file_manager.h"
#include "handlers/custom_handler.h"
#include "tools/dvalue.h"

using namespace NAF;


class Frontend : public Handlers::RootHandler
{
    public:
        Frontend() : RootHandler(){}

        void Process_()
        {
            file_manager_.set_directory_for_uploaded_files("/var/www/uploaded-files");
            file_manager_.AddSupportedFile_("png", Files::FileProperties{"image/png", true, {""}});
            auto method = GetMethod_(get_properties().method);
            switch(method)
            {
                case HTTP::EnumMethods::kHTTP_POST:
                    UploadProcess_();
                    break;
                case HTTP::EnumMethods::kHTTP_GET:
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

        void UploadProcess_()
        {
            // Manage the files
                auto& request = get_http_server_request().value();
                file_manager_.set_operation_type(Files::OperationType::kUpload);
                HTMLForm form(*request, request->stream(), file_manager_);

            // Verify supported files
                if(!file_manager_.IsSupported_())
                {
                    HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
                    return;
                }

            // Verify max file size
                if(!file_manager_.VerifyMaxFileSize_())
                {
                    HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The requested file exceeds the file size limit.");
                    return;
                }

            // Upload file
                file_manager_.UploadFile_();

            // Response
                CompoundFillResponse_(HTTP::Status::kHTTP_OK, file_manager_.get_result(), "Ok.");
        }

    private:
        Files::FileManager file_manager_;
};

int main(int argc, char** argv)
{
    Core::NebulaAtom app;
    Query::DatabaseManager::StartMySQL_();
    Tools::SettingsManager::ReadBasicProperties_();


    app.AddHandler_("/", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, std::string(" \
                <html><head><title>Upload file example</title></head> \
                <body><input type=\"file\" id=\"image-file\"><button onclick=\"upload()\">Upload</button><script> \
                    function upload(){ \
                        let photo = document.getElementById(\"image-file\").files[0]; \
                        let formData = new FormData(); \
                        formData.append(\"photo\", photo); \
                        fetch('/upload', {method: \"POST\", body: formData}); \
                    } \
                </script></body> \
            "));
        });
    });

    app.AddHandler_("/upload", [&]()
    {
        return new Frontend;
    });

    app.AddHandler_("/database", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, std::string(" \
                <html><head><title>Upload file example</title></head> \
                <body><input type=\"file\" id=\"image-file\"><button onclick=\"upload()\">Upload</button><script> \
                    function upload(){ \
                        let photo = document.getElementById(\"image-file\").files[0]; \
                        let formData = new FormData(); \
                        formData.append(\"photo\", photo); \
                        fetch('/database/upload', {method: \"POST\", body: formData \
                        ,mode: \"no-cors\", credentials: \"same-origin\" \
                        }); \
                    } \
                </script></body> \
            "));
        });
    });

    app.AddHandler_("/database/upload", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.ManageRequestBody_();
            Functions::Action a1("a1");
            a1.set_sql_code("INSERT INTO test_files (file) VALUES (?)");
            a1.AddParameter_("photo", Tools::DValue(""), true);

            a1.IdentifyParameters_(self.get_files_parameters());
            a1.ComposeQuery_();
            a1.ExecuteQuery_();

            if(a1.get_error())
                self.HTMLResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, a1.get_custom_error());
            else
                self.HTMLResponse_(HTTP::Status::kHTTP_OK, "OK");
        });
    });


    return app.Init_(argc, argv);

}
