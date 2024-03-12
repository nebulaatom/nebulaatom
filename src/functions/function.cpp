
#include "functions/function.h"

using namespace CPW;
using namespace AtomFunctions;

Function::Function() :
    endpoint_("")
    ,endpoint2_("")
    ,target_("")
    ,type_(Type::kGET)
{
    methods_.insert({"POST", Type::kPOST});
    methods_.insert({"GET", Type::kGET});
    methods_.insert({"PUT", Type::kPUT});
    methods_.insert({"DEL", Type::kDEL});
}

Function::Function(std::string endpoint, Type type) :
    endpoint_(endpoint)
    ,endpoint2_("")
    ,target_("")
    ,type_(type)
{
    methods_.insert({"POST", Type::kPOST});
    methods_.insert({"GET", Type::kGET});
    methods_.insert({"PUT", Type::kPUT});
    methods_.insert({"DEL", Type::kDEL});
}
