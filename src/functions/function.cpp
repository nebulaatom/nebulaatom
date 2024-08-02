
#include "functions/function.h"
#include "functions/action.h"
#include "http/methods.h"

using namespace NAF;
using namespace NAF::Functions;

Function::Function() :
    endpoint_("")
    ,target_("")
    ,method_(HTTP::EnumMethods::kHTTP_GET)
{
    
}

Function::Function(std::string endpoint, HTTP::EnumMethods method) :
    endpoint_(endpoint)
    ,target_("")
    ,method_(method)
{
    
}

Action::Ptr Function::AddAction_(std::string identifier)
{
    Action::Ptr action = std::make_shared<Action>(identifier);
    actions_.push_back(action);
    return action;
}
