
#include "results.h"

using namespace CPW::Query;

Field::Field(std::string column_name, Tools::RowValueFormatter value) :
    column_name_(column_name)
    ,value_(value)
{

}

Row::Row()
{

}

Field Row::FindField_(std::string column_name)
{
    auto field = std::find_if(fields_.begin(), fields_.end(), [column_name](Field field)
    {
        return field.get_column_name() == column_name;
    });

    if(field == fields_.end())
        return Field("", Tools::RowValueFormatter(std::string("")));

    return *field;
}

Results::Results()
{

}
