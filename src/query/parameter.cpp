
#include "query/parameter.h"
#include "query/results.h"

using namespace CPW::Query;

Parameter::Parameter(std::string name, Tools::RowValueFormatter value, bool editable) :
    parameter_type_(ParameterType::kField)
    ,name_(name)
    ,editable_(editable)
    ,value_(value)
    ,conditional_field_(0, 0)
{
    result_ = std::make_shared<Query::Results>();
}

Parameter::Parameter(std::string name, Query::ConditionalField conditional_field, std::shared_ptr<Query::Results> result, bool editable) :
    parameter_type_(ParameterType::kConditional)
    ,name_(name)
    ,editable_(editable)
    ,value_(Tools::RowValueFormatter(0))
    ,conditional_field_(conditional_field)
    ,result_(result)
{

}