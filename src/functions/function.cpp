
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

        // Set JSON results
        if(action->get_final())
            json_result = action->get_json_result();
    }

    return true;
}

bool Function::ProcessFile_(std::string& file_path)
{
    // Process actions of the function
    Tools::OutputLogger::Debug_("Function: " + endpoint_);
    for(auto& action : actions_)
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

        // Set JSON results
        if(action->get_final())
        {
            Query::Field::Position position(0, 0);
            Query::Field::Ptr field = action->get_results()->FindField_(position);
            if(field != nullptr)
                file_path = field->String_();
        }
    }

    return true;
}