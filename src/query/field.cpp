
#include "field.h"

using namespace Atom::Query;

Field::Field(std::string column_name, Tools::DValue value) :
    column_name_(column_name)
    ,value_(value)
{

}
