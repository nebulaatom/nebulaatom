
#include "core/nebula_atom.h"
#include "files/file_manager.h"
#include "handlers/custom_handler.h"
#include "handlers/frontend_handler.h"

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
            HandlePOSTMethod_();
        }
};

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

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

    return app.Init_(argc, argv);

}
