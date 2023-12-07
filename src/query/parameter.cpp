
#include "query/parameter.h"

using namespace CPW::Query;

Parameter::Parameter(std::string name, Tools::RowValueFormatter value, bool editable) :
    name_(name)
    ,editable_(editable)
    ,value_(value)
{

}
