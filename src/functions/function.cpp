
#include "functions/function.h"
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
