
#include "core/nebula_atom.h"
#include "files/file_manager.h"
#include "handlers/custom_handler.h"
#include <Poco/File.h>

using namespace Atom;


int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    app.AddHandler_("/", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.CustomHTMLResponse_(HTTP::Status::kOK, std::string(" \
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
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            // Manage the files
                Files::FileManager file_manager(Files::OperationType::kUpload);
                file_manager.set_directory_for_uploaded_files("/var/www");
                HTMLForm form(*self.get_request(), self.get_request()->stream(), file_manager);

            // Verify supported files
                file_manager.AddSupportedFile_("png", Files::FileProperties{"image/png", true, {""}});
                if(!file_manager.IsSupported_())
                {
                    self.HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
                    return;
                }

            // Upload file
                file_manager.UploadFile_();
                self.JSONResponse_(HTTP::Status::kOK, "Ok.");
        });
    });

    app.AddHandler_("/download", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            // Manage the file
                Files::FileManager file_manager(Files::OperationType::kDownload);
                file_manager.get_files().push_back(file_manager.CreateTempFile_(self.get_request()->getURI()));
                auto& tmp_file = file_manager.get_files().front();

            // Check file
                if(!file_manager.CheckFiles_())
                {
                    self.HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
                    return;
                }

            // Is supported
                file_manager.AddSupportedFile_("png", Files::FileProperties{"image/png", true, {""}});
                if(!file_manager.IsSupported_(tmp_file))
                {
                    self.HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
                    return;
                }

            // Reponse
                self.get_response()->setStatus(HTTPResponse::HTTP_OK);
                self.get_response()->setContentType(tmp_file.get_content_type());
                self.get_response()->setContentLength(tmp_file.get_content_length());
                std::ostream& out_reponse = self.get_response()->send();

            // Download file
                file_manager.DownloadFile_(out_reponse);
                out_reponse.flush();

        });
    });

    app.AddHandler_("/modify", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            // Manage the file
                Files::FileManager file_manager(Files::OperationType::kDelete);
                file_manager.get_files().push_back(file_manager.CreateTempFile_(self.get_request()->getURI()));

            // Check file
                if(!file_manager.CheckFiles_())
                {
                    self.HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
                    return;
                }

            // Remove the file
                file_manager.RemoveFile_();
                file_manager.get_files().clear();
                file_manager.get_result()->clear();

            // Upload new file
                file_manager.set_operation_type(Files::OperationType::kUpload);
                HTMLForm form(*self.get_request(), self.get_request()->stream(), file_manager);

                if(!file_manager.IsSupported_())
                {
                    self.HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
                    return;
                }
                self.JSONResponse_(HTTP::Status::kOK, "Ok.");
        });
    });

    app.AddHandler_("/remove", [&]()
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            // Manage file
                Files::FileManager file_manager(Files::OperationType::kDelete);
                file_manager.get_files().push_back(file_manager.CreateTempFile_(self.get_request()->getURI()));

            // Basic operations
                if(!file_manager.CheckFiles_())
                {
                    self.HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
                    return;
                }

                file_manager.RemoveFile_();
                self.JSONResponse_(HTTP::Status::kOK, "Ok.");
        });
    });

    return app.run(argc, argv);

}
