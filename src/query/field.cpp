
#include "field.h"

using namespace NAF::Query;

Field::Field() :
    is_null_(true)
    ,column_name_("")
    ,value_(Tools::DValue())
{

}

Field::Field(std::string column_name, Tools::DValue value) :
    is_null_(false)
    ,column_name_(column_name)
    ,value_(value)
{
    
}

bool Field::IsNull_()
{
    return is_null_;
}

std::string Field::ToString_()
{
    return value_.ToString_();
}

std::string& Field::String_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kString))
    {
        throw std::runtime_error("Error on field.cpp on String_(): The data type to be obtained does not match the current data type.");
    }

    return value_.String_();
}

float& Field::Float_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kFloat))
    {
        throw std::runtime_error("Error on field.cpp on Float_(): The data type to be obtained does not match the current data type.");
    }

    return value_.Float_();
}

bool& Field::Bool_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kBoolean))
    {
        throw std::runtime_error("Error on field.cpp on String_(): The data type to be obtained does not match the current data type.");
    }

    return value_.Bool_();
}

int& Field::Int_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kInteger))
    {
        throw std::runtime_error("Error on field.cpp on String_(): The data type to be obtained does not match the current data type.");
    }

    return value_.Int_();
}
