/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2023  Jose F Rivas C <josefelixrc7@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPW_QUERY_CONDITION_H
#define CPW_QUERY_CONDITION_H


#include <string>

#include "tools/row_value_formatter.h"
#include "query/results.h"


namespace CPW
{
    namespace Query
    {
        enum class ConditionType;
        class Condition;
    }
}


enum class CPW::Query::ConditionType
{
    kIqual
    ,kNoIqual
    ,kGreatherThan
    ,kSmallerThan
    ,kList
};

class CPW::Query::Condition
{
    public:
        Condition(ConditionType type, Field field_condition, ConditionalField field_value);
        Condition(ConditionType type, Field field_condition, std::vector<ConditionalField> field_values);

        ConditionType get_type() const { return type_; }
        Field& get_field_condition()
        {
            auto& var = field_condition_;
            return var;
        }
        ConditionalField& get_field_value()
        {
            auto& var = field_value_;
            return var;
        }
        std::vector<ConditionalField>& get_field_values()
        {
            auto& var = field_values_;
            return var;
        }

        void set_type(ConditionType type) { type_ = type; }

        bool VerifyCondition_(Results& results);

    private:
        ConditionType type_;
        Field field_condition_;
        ConditionalField field_value_;
        std::vector<ConditionalField> field_values_;
};

#endif // CPW_QUERY_CONDITION_H
