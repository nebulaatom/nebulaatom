
#include "functions/function.h"
#include "http/methods.h"

using namespace Atom;
using namespace Atom::Functions;

Function::Function() :
    endpoint_("")
    ,endpoint2_("")
    ,target_("")
    ,type_(HTTP::EnumMethods::kHTTP_GET)
{
    
}

Function::Function(std::string endpoint, HTTP::EnumMethods type) :
    endpoint_(endpoint)
    ,endpoint2_("")
    ,target_("")
    ,type_(type)
{
    
}
