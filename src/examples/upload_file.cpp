
#include "core/nebula_atom.h"
#include "files/file_manager.h"
#include "handlers/custom_handler.h"
#include "handlers/frontend_handler.h"
#include "tools/dvalue.h"

using namespace Atom;

class UploadFile : public Handlers::FrontendHandler
{
    public:
        UploadFile(){}
        virtual ~UploadFile(){}

        void Process_()
        {
            get_file_manager().set_directory_for_uploaded_files("/var/www/uploaded-files");
            get_file_manager().AddSupportedFile_("png", Files::FileProperties{"image/png", true, {""}});
            UploadProcess_();
        }
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
        return new UploadFile;
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
