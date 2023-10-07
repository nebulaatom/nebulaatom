
#include "query/parameter.h"

using namespace CPW::Query;

Parameter::Parameter(std::string name, Tools::RowValueFormatter value) :
    name_(name)
    ,value_(value)
{

}
