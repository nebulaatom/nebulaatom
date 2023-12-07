
#include "query/condition.h"

using namespace CPW::Query;

Condition::Condition(ConditionType type, Field field_condition, ConditionalField field_value) :
    type_(type)
    ,field_condition_(field_condition)
    ,field_value_(field_value)
{

}

Condition::Condition(ConditionType type, Field field_condition, std::vector<ConditionalField> field_values) :
    type_(type)
    ,field_condition_(field_condition)
    ,field_value_(0, 0)
    ,field_values_(field_values)
{

}

bool Condition::VerifyCondition_(Results& results)
{
    switch(type_)
    {
        case ConditionType::kIqual:
        {
            auto field = results.FindField_(field_value_);
            if(field == nullptr)
                return false;

            if(field_condition_.get_value().get_row_value_type() != field->get_value().get_row_value_type())
                return false;

            if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kBoolean)
                return field_condition_.get_value().get_value_bool() == field->get_value().get_value_bool();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kEmpty)
                return false;
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kFloat)
                return field_condition_.get_value().get_value_float() == field->get_value().get_value_float();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kInteger)
                return field_condition_.get_value().get_value_int() == field->get_value().get_value_int();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kString)
                return field_condition_.get_value().get_value_string() == field->get_value().get_value_string();
            else
                return false;

            break;
        }
        case ConditionType::kNoIqual:
        {
            auto field = results.FindField_(field_value_);
            if(field == nullptr)
                return false;

            if(field_condition_.get_value().get_row_value_type() != field->get_value().get_row_value_type())
                return false;

            if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kBoolean)
                return field_condition_.get_value().get_value_bool() != field->get_value().get_value_bool();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kEmpty)
                return false;
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kFloat)
                return field_condition_.get_value().get_value_float() != field->get_value().get_value_float();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kInteger)
                return field_condition_.get_value().get_value_int() != field->get_value().get_value_int();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kString)
                return field_condition_.get_value().get_value_string() != field->get_value().get_value_string();
            else
                return false;

            break;
        }
        case ConditionType::kGreatherThan:
        {
            auto field = results.FindField_(field_value_);
            if(field == nullptr)
                return false;

            if(field_condition_.get_value().get_row_value_type() != field->get_value().get_row_value_type())
                return false;

            if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kBoolean)
                return field->get_value().get_value_bool() > field_condition_.get_value().get_value_bool();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kEmpty)
                return false;
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kFloat)
                return field->get_value().get_value_float() > field_condition_.get_value().get_value_float();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kInteger)
                return field->get_value().get_value_int() > field_condition_.get_value().get_value_int();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kString)
                return field->get_value().get_value_string() > field_condition_.get_value().get_value_string();
            else
                return false;

            break;
        }
        case ConditionType::kSmallerThan:
        {
            auto field = results.FindField_(field_value_);
            if(field == nullptr)
                return false;

            if(field_condition_.get_value().get_row_value_type() != field->get_value().get_row_value_type())
                return false;

            if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kBoolean)
                return field->get_value().get_value_bool() < field_condition_.get_value().get_value_bool();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kEmpty)
                return false;
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kFloat)
                return field->get_value().get_value_float() < field_condition_.get_value().get_value_float();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kInteger)
                return field->get_value().get_value_int() < field_condition_.get_value().get_value_int();
            else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kString)
                return field->get_value().get_value_string() < field_condition_.get_value().get_value_string();
            else
                return false;

            break;
        }
        case ConditionType::kList:
        {
            for(auto& value : field_values_)
            {
                auto field = results.FindField_(value);
                if(field == nullptr)
                    return false;

                if(field_condition_.get_value().get_row_value_type() != field->get_value().get_row_value_type())
                    continue;

                if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kBoolean)
                {
                    if(field_condition_.get_value().get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kEmpty)
                    return false;
                else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kFloat)
                {
                    if(field_condition_.get_value().get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kInteger)
                {
                    if(field_condition_.get_value().get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(field_condition_.get_value().get_row_value_type() == Tools::RowValueType::kString)
                {
                    if(field_condition_.get_value().get_value_bool() == field->get_value().get_value_bool())
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
