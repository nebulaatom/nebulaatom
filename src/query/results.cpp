
#include "query/results.h"

using namespace Atom::Query;

Results::Results()
{

}

Field::Ptr Results::FindField_(Field::Position& field_position)
{
    Field::Ptr field_result = nullptr;

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

        auto field_value = row->at(field_position.column);
        return field_value;
}

Row::Ptr Results::AddRow_()
{
    Row::Ptr row = std::make_shared<Row>();
    push_back(row);
    return row;
}