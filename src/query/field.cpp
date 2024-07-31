
#include "field.h"

using namespace Atom::Query;

Field::Field(std::string column_name, Tools::DValue value) :
    column_name_(column_name)
    ,value_(value)
{

}

std::string Field::String_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kString))
    {
        throw std::runtime_error("Error on field.cpp on String_(): The data type to be obtained does not match the current data type.");
    }

    return value_.get_value_string();
}

float Field::Float_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kFloat))
    {
        throw std::runtime_error("Error on field.cpp on Float_(): The data type to be obtained does not match the current data type.");
    }

    return value_.get_value_float();
}

bool Field::Bool_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kBoolean))
    {
        throw std::runtime_error("Error on field.cpp on String_(): The data type to be obtained does not match the current data type.");
    }

    return value_.get_value_bool();
}

int Field::Int_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kInteger))
    {
        throw std::runtime_error("Error on field.cpp on String_(): The data type to be obtained does not match the current data type.");
    }

    return value_.get_value_int();
}
