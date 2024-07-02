
#include "query/row.h"

using namespace Atom::Query;

Row::Row()
{

}

Field::Ptr Row::FindField_(std::string column_name)
{
    Field::Ptr field_result = nullptr;
    auto field_value = std::find_if(begin(), end(), [column_name](Field::Ptr field)
    {
        return field->get_column_name() == column_name;
    });

    if(field_value == end())
        return field_result;

    return *field_value;
}

Field::Ptr Row::AddField_(std::string column_name, Tools::DValue value)
{
    Field::Ptr field = std::make_shared<Field>(column_name, value);
    push_back(field);
    return field;
}