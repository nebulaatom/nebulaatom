
#include "functions/function.h"

using namespace CPW;
using namespace CPW::Functions;

Function::Function(std::string endpoint, Type type) :
    endpoint_(endpoint)
    ,type_(type)
{
    methods_.insert({"POST", Type::kPOST});
    methods_.insert({"GET", Type::kGET});
    methods_.insert({"PUT", Type::kPUT});
    methods_.insert({"DEL", Type::kDEL});
}
