
#include "results.h"
#include "tools/row_value_formatter.h"

using namespace Atom::Query;

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

std::shared_ptr<Field> Results::FindField_(ConditionalField& field)
{
    auto field_result = std::make_shared<Field>("", Tools::RowValueFormatter{});

    // Verify rows size
        if(rows_.size() < 1)
            return field_result;

        if(field.get_row() > rows_.size() - 1)
            return field_result;
        auto& row = rows_[field.get_row()].get_fields();

    // Verify specific row size
        if(row.size() < 1)
            return field_result;

        if(field.get_column() > row.size() - 1)
            return field_result;
        auto& field_value = row[field.get_column()];

    // Return final field
        field_result = std::make_shared<Field>(field_value);
        return field_result;
}
