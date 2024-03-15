
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    // Setting up handler
        app.CustomHandlerCreator_([&](const HTTPServerRequest&)
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
                    file_manager.AddBasicSupportedFiles_();
                    if(!file_manager.IsSupported_())
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

        return app.run(argc, argv);

}
