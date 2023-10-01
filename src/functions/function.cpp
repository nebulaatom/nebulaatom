
#include "functions/function.h"

using namespace CPW;
using namespace CPW::Functions;

Function::Function(std::string endpoint) :
    endpoint_(endpoint)
{
    filters_ = std::make_shared<Filters::FiltersManager>();
    triggers_ = std::make_shared<Filters::FiltersManager>();
}
