
#include "query/parameter.h"
#include "query/results.h"

using namespace AtomQuery;

Parameter::Parameter(std::string name, Tools::RowValueFormatter value, bool editable) :
    parameter_type_(ParameterType::kField)
    ,name_(name)
    ,editable_(editable)
    ,value_(value)
    ,conditional_field_(0, 0)
    ,conditional_field_action_("")
{
    
}

Parameter::Parameter(std::string name, Query::ConditionalField conditional_field, std::string conditional_field_action, bool editable) :
    parameter_type_(ParameterType::kConditional)
    ,name_(name)
    ,editable_(editable)
    ,value_(Tools::RowValueFormatter(0))
    ,conditional_field_(conditional_field)
    ,conditional_field_action_(conditional_field_action)
{

}