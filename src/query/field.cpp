
#include "field.h"

using namespace Atom::Query;

Field::Field(std::string column_name, Tools::RowValueFormatter value) :
    column_name_(column_name)
    ,value_(value)
{

}
