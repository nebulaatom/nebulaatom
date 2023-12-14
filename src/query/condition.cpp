
#include "query/condition.h"

using namespace CPW::Query;

Condition::Condition(ConditionType type, Tools::RowValueFormatter row_value, ConditionalField field_value) :
    type_(type)
    ,row_value_(row_value)
    ,field_value_(field_value)
{

}

Condition::Condition(Tools::RowValueFormatter row_value, std::vector<ConditionalField> field_values) :
    type_(ConditionType::kList)
    ,row_value_(row_value)
    ,field_value_(0, 0)
    ,field_values_(field_values)
{

}

bool Condition::VerifyCondition_(std::shared_ptr<Results>& results)
{
    switch(type_)
    {
        case ConditionType::kIqual:
        {
            auto field = results->FindField_(field_value_);
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
            auto field = results->FindField_(field_value_);
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
            auto field = results->FindField_(field_value_);
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
            auto field = results->FindField_(field_value_);
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
            for(auto& value : field_values_)
            {
                auto field = results->FindField_(value);
                if(field == nullptr)
                    return false;

                if(row_value_.get_row_value_type() != field->get_value().get_row_value_type())
                    continue;

                if(row_value_.get_row_value_type() == Tools::RowValueType::kBoolean)
                {
                    if(row_value_.get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(row_value_.get_row_value_type() == Tools::RowValueType::kEmpty)
                    return false;
                else if(row_value_.get_row_value_type() == Tools::RowValueType::kFloat)
                {
                    if(row_value_.get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(row_value_.get_row_value_type() == Tools::RowValueType::kInteger)
                {
                    if(row_value_.get_value_bool() == field->get_value().get_value_bool())
                        return true;
                    else
                        continue;
                }
                else if(row_value_.get_row_value_type() == Tools::RowValueType::kString)
                {
                    if(row_value_.get_value_bool() == field->get_value().get_value_bool())
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
