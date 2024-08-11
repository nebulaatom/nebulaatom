
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