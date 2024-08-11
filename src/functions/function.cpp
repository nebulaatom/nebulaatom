
#include "functions/function.h"
#include "functions/action.h"
#include "http/methods.h"

using namespace NAF;
using namespace NAF::Functions;

Function::Function() :
    endpoint_("")
    ,target_("")
    ,error_(false)
    ,error_message_("Unknown error.")
    ,response_type_(ResponseType::kJSON)
    ,method_(HTTP::EnumMethods::kHTTP_GET)
{
    
}

Function::Function(std::string endpoint, HTTP::EnumMethods method, ResponseType response_type) :
    endpoint_(endpoint)
    ,target_("")
    ,error_(false)
    ,error_message_("Unknown error.")
    ,response_type_(response_type)
    ,method_(method)
{
    
}

Action::Ptr Function::AddAction_(std::string identifier)
{
    Action::Ptr action = std::make_shared<Action>(identifier);
    actions_.push_back(action);
    return action;
}

void Function::Setup_(HTTP::Request::HTTPServerRequestPtr request, HTTP::Request::HTTPServerResponsePtr response)
{
    if(request.has_value())
        SetupRequest_(*request.value());
    else
    {
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error to setup Request in Function");
        return;
    }
    if(response.has_value())
        SetupResponse_(*response.value());
    else
    {
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error to setup Response in Function");
        return;
    }
}
void Function::Process_(HTTP::Request::HTTPServerRequestPtr request, HTTP::Request::HTTPServerResponsePtr response)
{
    Setup_(request, response);

    switch(response_type_)
    {
        case Functions::Function::ResponseType::kJSON:
        {
            JSON::Object::Ptr json_result = new JSON::Object();
            if(!ProcessJSON_(json_result))
            {
                if(error_)
                {
                    JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, error_message_);
                    return;
                }
            }

            // Send JSON results
            CompoundResponse_(HTTP::Status::kHTTP_OK, json_result);

            break;
        }
        case Functions::Function::ResponseType::kFile:
        {
            std::string filepath = "";
            if(!ProcessFile_(filepath))
            {
                if(error_)
                {
                    JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, error_message_);
                    return;
                }
            }

            switch(method_)
            {
                case HTTP::EnumMethods::kHTTP_GET:
                    DownloadProcess_(filepath);
                    break;
                case HTTP::EnumMethods::kHTTP_POST:
                    UploadProcess_();
                    break;
                case HTTP::EnumMethods::kHTTP_PUT:
                    ModifyProcess_(filepath);
                    break;
                case HTTP::EnumMethods::kHTTP_DEL:
                    RemoveProcess_(filepath);
                    break;
                case HTTP::EnumMethods::kHTTP_HEAD:
                case HTTP::EnumMethods::kHTTP_OPTIONS:
                case HTTP::EnumMethods::kHTTP_PATCH:
                case HTTP::EnumMethods::kNULL:
                    JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The client provided a bad HTTP method.");
                    break;
            }
            
            break;
        }
    }
}

bool Function::ProcessAction_(Action::Ptr action)
{
    Tools::OutputLogger::Debug_("Action: " + action->get_identifier() + ", Final: " + std::to_string(action->get_final()));

    // Copy actions references
    action->get_actions_container().clear();
    action->get_actions_container().insert(action->get_actions_container().end(), actions_.begin(), actions_.end());
    
    // Identify parameters
    if(action->get_error())
    {
        error_ = true;
        error_message_ = action->get_custom_error();
        return false;
    }

    // Execute action
    if(!action->Work_())
    {
        error_ = true;
        error_message_ = action->get_custom_error();
        return false;
    }
    return true;
}

bool Function::ProcessJSON_(JSON::Object::Ptr& json_result)
{
    // Process actions of the function
    Tools::OutputLogger::Debug_("Function: " + endpoint_);
    for(auto& action : actions_)
    {
        // Process Action
        ProcessAction_(action);

        // Set JSON results
        if(action->get_final())
            json_result = action->get_json_result();
    }

    return true;
}

bool Function::ProcessFile_(std::string& filepath)
{
    // Process actions of the function
    Tools::OutputLogger::Debug_("Function: " + endpoint_);
    for(auto& action : actions_)
    {
        // Process Action
        ProcessAction_(action);

        // Set filepath
        if(action->get_final())
        {
            Query::Field::Position position(0, 0);
            Query::Field::Ptr field = action->get_results()->FindField_(position);
            if(field != nullptr)
                filepath = field->String_();
        }
    }

    return true;
}

void Function::DownloadProcess_(std::string& filepath)
{
    // Manage the file
    file_manager_->set_operation_type(Files::OperationType::kDownload);
    file_manager_->get_files().push_back(file_manager_->CreateTempFile_("/" + filepath));
    auto& tmp_file = file_manager_->get_files().front();

    // Check file
    if(!file_manager_->CheckFiles_())
    {
        HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
        return;
    }

    // Is supported
    if(!file_manager_->IsSupported_(tmp_file))
    {
        HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
        return;
    }
    file_manager_->ProcessContentLength_();

    // Reponse
    auto& response = get_http_server_response().value();
    response->setStatus(HTTPResponse::HTTP_OK);
    response->setContentType(tmp_file.get_content_type());
    response->setContentLength(tmp_file.get_content_length());
    
    std::ostream& out_reponse = response->send();

    // Download file
    file_manager_->DownloadFile_(out_reponse);
}

void Function::UploadProcess_()
{
    // Manage the files
    file_manager_->set_operation_type(Files::OperationType::kUpload);

    // Change path
    for(auto& file : file_manager_->get_files())
    {
        if(!file_manager_->ChangePathAndFilename_(file, file_manager_->get_directory_base()))
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The file cannot be processed.");
            return;
        }
    }

    // Verify supported files
    if(!file_manager_->IsSupported_())
    {
        HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
        return;
    }

    // Verify max file size
    if(!file_manager_->VerifyMaxFileSize_())
    {
        HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The requested file exceeds the file size limit.");
        return;
    }

    // Upload file
    file_manager_->UploadFile_();

    // Response
    CompoundFillResponse_(HTTP::Status::kHTTP_OK, file_manager_->get_result(), "Ok.");
}

void Function::ModifyProcess_(std::string& filepath)
{
    // Manage the file
    Files::FileManager tmp_file_manager = Files::FileManager(*file_manager_);
    tmp_file_manager.set_operation_type(Files::OperationType::kDelete);

    tmp_file_manager.get_files().clear();
    tmp_file_manager.get_files().push_back(file_manager_->CreateTempFile_("/" + filepath));

    // Check file
    if(!tmp_file_manager.CheckFiles_())
    {
        HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
        return;
    }

    // Remove the file
    tmp_file_manager.RemoveFile_();

    // Process new file to upload
    file_manager_->set_operation_type(Files::OperationType::kUpload);
    
    // Change path
    for(auto& file : file_manager_->get_files())
    {
        if(!file_manager_->ChangePathAndFilename_(file, file_manager_->get_directory_base()))
        {
            HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The file cannot be processed.");
            return;
        }
    }

    // Is supported
    if(!file_manager_->IsSupported_())
    {
        HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Requested file is not supported.");
        return;
    }
    
    // Verify max file size
    if(!file_manager_->VerifyMaxFileSize_())
    {
        HTMLResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "The requested file exceeds the file size limit.");
        return;
    }

    // Upload
    file_manager_->UploadFile_();

    // Response
    CompoundFillResponse_(HTTP::Status::kHTTP_OK, file_manager_->get_result(), "Ok.");
}

void Function::RemoveProcess_(std::string& filepath)
{
    // Manage file
    file_manager_->set_operation_type(Files::OperationType::kDelete);
    file_manager_->get_files().push_back(file_manager_->CreateTempFile_("/" + filepath));

    // Basic operations
    if(!file_manager_->CheckFiles_())
    {
        HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Requested file bad check.");
        return;
    }

    // Remove
    file_manager_->RemoveFile_();
        
    // Response
    JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
}
