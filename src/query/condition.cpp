
#include "query/condition.h"
#include "tools/row_value_formatter.h"

using namespace CPW::Query;

Condition::Condition(ConditionType type, Tools::RowValueFormatter row_value, ConditionalField conditional_field) :
    type_(type)
    ,row_value_(row_value)
    ,conditional_field_(conditional_field)
{

}

Condition::Condition(std::vector<Tools::RowValueFormatter> row_values, ConditionalField conditional_field) :
    type_(ConditionType::kList)
    ,row_value_(Tools::RowValueFormatter(""))
    ,row_values_(row_values)
    ,conditional_field_(conditional_field)
{

}

bool Condition::VerifyCondition_(std::shared_ptr<Results>& results)
{
    switch(type_)
    {
        case ConditionType::kIqual:
        {
            auto field = results->FindField_(conditional_field_);
            if(field == nullptr)
                return false;

            if(row_value_.get_row_value_type() != field->get_value().get_row_value_type())
                return false;

            if(row_value_.get_row_value_type() == Tools::RowValueType::kBoolean)
                return row_value_.get_value_bool() == field->get_value().get_value_bool();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kEmpty)
                return false;
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kFloat)
                return row_value_.get_value_float() == field->get_value().get_value_float();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kInteger)
                return row_value_.get_value_int() == field->get_value().get_value_int();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kString)
                return row_value_.get_value_string() == field->get_value().get_value_string();
            else
                return false;

            break;
        }
        case ConditionType::kNoIqual:
        {
            auto field = results->FindField_(conditional_field_);
            if(field == nullptr)
                return false;

            if(row_value_.get_row_value_type() != field->get_value().get_row_value_type())
                return false;

            if(row_value_.get_row_value_type() == Tools::RowValueType::kBoolean)
                return row_value_.get_value_bool() != field->get_value().get_value_bool();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kEmpty)
                return false;
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kFloat)
                return row_value_.get_value_float() != field->get_value().get_value_float();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kInteger)
                return row_value_.get_value_int() != field->get_value().get_value_int();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kString)
                return row_value_.get_value_string() != field->get_value().get_value_string();
            else
                return false;

            break;
        }
        case ConditionType::kGreatherThan:
        {
            auto field = results->FindField_(conditional_field_);
            if(field == nullptr)
                return false;

            if(row_value_.get_row_value_type() != field->get_value().get_row_value_type())
                return false;

            if(row_value_.get_row_value_type() == Tools::RowValueType::kBoolean)
                return field->get_value().get_value_bool() > row_value_.get_value_bool();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kEmpty)
                return false;
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kFloat)
                return field->get_value().get_value_float() > row_value_.get_value_float();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kInteger)
                return field->get_value().get_value_int() > row_value_.get_value_int();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kString)
                return field->get_value().get_value_string() > row_value_.get_value_string();
            else
                return false;

            break;
        }
        case ConditionType::kSmallerThan:
        {
            auto field = results->FindField_(conditional_field_);
            if(field == nullptr)
                return false;

            if(row_value_.get_row_value_type() != field->get_value().get_row_value_type())
                return false;

            if(row_value_.get_row_value_type() == Tools::RowValueType::kBoolean)
                return field->get_value().get_value_bool() < row_value_.get_value_bool();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kEmpty)
                return false;
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kFloat)
                return field->get_value().get_value_float() < row_value_.get_value_float();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kInteger)
                return field->get_value().get_value_int() < row_value_.get_value_int();
            else if(row_value_.get_row_value_type() == Tools::RowValueType::kString)
                return field->get_value().get_value_string() < row_value_.get_value_string();
            else
                return false;

            break;
        }
        case ConditionType::kList:
        {
            auto field = results->FindField_(conditional_field_);
            if(field == nullptr)
                return false;

            for(auto& value : row_values_)
            {
                if(value.get_row_value_type() != field->get_value().get_row_value_type())
                    continue;

                if(value.get_row_value_type() == Tools::RowValueType::kBoolean)
                {
                    if(value.get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(value.get_row_value_type() == Tools::RowValueType::kEmpty)
                    return false;
                else if(value.get_row_value_type() == Tools::RowValueType::kFloat)
                {
                    if(value.get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(value.get_row_value_type() == Tools::RowValueType::kInteger)
                {
                    if(value.get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(value.get_row_value_type() == Tools::RowValueType::kString)
                {
                    if(value.get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
            }

            return false;

            break;
        }
    }
}
