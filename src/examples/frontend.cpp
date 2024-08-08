
#include "core/nebula_atom.h"
#include "functions/action.h"
#include "handlers/custom_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"
#include "files/file_manager.h"

using namespace NAF;
using namespace NAF::Handlers;

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
                    UploadProcess_();
                    break;
                case HTTP::EnumMethods::kHTTP_PUT:
                    ModifyProcess_();
                    break;
                case HTTP::EnumMethods::kHTTP_DEL:
                    RemoveProcess_();
                    break;
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

        void ModifyProcess_()
        {
            // Manage the file
                auto& request = get_http_server_request().value();
                file_manager_.set_operation_type(Files::OperationType::kDelete);
                file_manager_.get_files().push_back(file_manager_.CreateTempFile_(request->getURI()));

            // Check file
                if(!file_manager_.CheckFiles_())
                {
                    HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
                    return;
                }

            // Remove the file
                file_manager_.RemoveFile_();
                file_manager_.get_files().clear();
                file_manager_.get_result()->clear();

            // Process new file to upload
                file_manager_.set_operation_type(Files::OperationType::kUpload);
                HTMLForm form(*request, request->stream(), file_manager_);

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

            // Upload
                file_manager_.UploadFile_();

            // Response
                CompoundFillResponse_(HTTP::Status::kHTTP_OK, file_manager_.get_result(), "Ok.");
        }

        void RemoveProcess_()
        {
            // Manage file
                auto& request = get_http_server_request().value();
                file_manager_.set_operation_type(Files::OperationType::kDelete);
                file_manager_.get_files().push_back(file_manager_.CreateTempFile_(request->getURI()));

            // Basic operations
                if(!file_manager_.CheckFiles_())
                {
                    HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
                    return;
                }

            // Remove
                file_manager_.RemoveFile_();
                
            // Response
                JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
        }

    private:
        Files::FileManager file_manager_;
};

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    Tools::SettingsManager::ReadBasicProperties_();

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo&)
    {
        return new Frontend();
    });

    return app.Init_(argc, argv);
}
