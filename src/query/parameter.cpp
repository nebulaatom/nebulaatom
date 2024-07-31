
#include "query/parameter.h"

using namespace Atom::Query;

Parameter::Parameter(std::string name, Tools::DValue value, bool editable) :
    parameter_type_(ParameterType::kField)
    ,name_(name)
    ,error_("")
    ,editable_(editable)
    ,value_(value)
    ,field_position_(0, 0)
    ,related_action_("")
{
    
}

Parameter::Parameter(std::string name, Query::Field::Position field_position, std::string related_action, bool editable) :
    parameter_type_(ParameterType::kPosition)
    ,name_(name)
    ,error_("")
    ,editable_(editable)
    ,value_(Tools::DValue(""))
    ,field_position_(field_position)
    ,related_action_(related_action)
{
    
}

std::string Parameter::ToString_()
{
    return value_.ToString_();
}

std::string Parameter::StringValue_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kString))
    {
        throw std::runtime_error("Error on Parameter.cpp on StringValue_(): The data type to be obtained does not match the current data type.");
    }

    return value_.get_value_string();
}

int Parameter::IntValue_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kInteger))
    {
        throw std::runtime_error("Error on Parameter.cpp on IntValue_(): The data type to be obtained does not match the current data type.");
    }

    return value_.get_value_int();
}

bool Parameter::BoolValue_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kBoolean))
    {
        throw std::runtime_error("Error on Parameter.cpp on BoolValue_(): The data type to be obtained does not match the current data type.");
    }

    return value_.get_value_bool();
}

float Parameter::FloatValue_()
{
    if(!value_.TypeIsIqual_(Tools::DValue::Type::kFloat))
    {
        throw std::runtime_error("Error on Parameter.cpp on FloatValue_(): The data type to be obtained does not match the current data type.");
    }

    return value_.get_value_float();
}

void Parameter::SetupCondition_(std::string identifier, Query::ConditionType type, Query::Condition<Ptr>::Functor functor)
{
    condition_ = std::make_shared<Query::Condition<Ptr>>(identifier, type, functor);
}