
#include "query/parameter.h"
#include "query/results.h"

using namespace Atom::Query;

Parameter::Parameter(std::string name, Tools::DValue value, bool editable) :
    parameter_type_(ParameterType::kField)
    ,name_(name)
    ,editable_(editable)
    ,value_(value)
    ,field_position_(0, 0)
    ,related_action_("")
{
    
}

Parameter::Parameter(std::string name, Query::Field::Position field_position, std::string related_action, bool editable) :
    parameter_type_(ParameterType::kPosition)
    ,name_(name)
    ,editable_(editable)
    ,value_(Tools::DValue(0))
    ,field_position_(field_position)
    ,related_action_(related_action)
{

}