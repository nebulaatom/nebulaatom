
#include "query/results.h"

using namespace NAF::Query;

Results::Results()
{

}

Field::Ptr Results::First_()
{
    Field::Ptr field_result = std::make_shared<Field>();
    return ExtractField_(0, 0);
}

Field::Ptr Results::Last_()
{
    Field::Ptr field_result = std::make_shared<Field>();
    std::size_t size_rows, size_fields;

    size_rows = size();
    if(size_rows < 1)
        return field_result;

    size_fields = at(size_rows - 1)->size();
    if(size_fields < 1)
        return field_result;

    return ExtractField_(size_rows, size_fields);
}

Field::Ptr Results::ExtractField_(std::size_t row, std::size_t column)
{
    Field::Position field_position(row, column);
    return ExtractField_(field_position);
}

Field::Ptr Results::ExtractField_(Field::Position& field_position)
{
    Field::Ptr field_result = std::make_shared<Field>();

    // Verify rows size
        if(size() < 1)
            return field_result;

        if(field_position.row > size() - 1)
            return field_result;

        auto& row = at(field_position.row);

    // Verify specific row size
        if(row->size() < 1)
            return field_result;

        if(field_position.column > row->size() - 1)
            return field_result;

        field_result = row->at(field_position.column);

    return field_result;
}

Row::Ptr Results::AddRow_()
{
    Row::Ptr row = std::make_shared<Row>();
    push_back(row);
    return row;
}