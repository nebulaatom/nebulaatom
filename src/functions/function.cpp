
#include "functions/function.h"
#include "functions/action.h"
#include "http/methods.h"

using namespace Atom;
using namespace Atom::Functions;

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

SQLAction::Ptr Function::AddSQLAction_(std::string identifier)
{
    SQLAction::Ptr action = std::make_shared<SQLAction>(identifier);
    actions_.push_back(action);
    return action;
}

EmailAction::Ptr Function::AddEmailAction_(std::string identifier)
{
    EmailAction::Ptr action = std::make_shared<EmailAction>(identifier);
    actions_.push_back(action);
    return action;
}