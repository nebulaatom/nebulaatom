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


#include "query/results.h"
#include "tools/row_value_formatter.h"


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
        Condition(ConditionType type, Tools::RowValueFormatter row_value, ConditionalField conditional_field);
        Condition(std::vector<Tools::RowValueFormatter> row_values, ConditionalField conditional_field);

        ConditionType get_type() const { return type_; }
        Tools::RowValueFormatter& get_row_value()
        {
            auto& var = row_value_;
            return var;
        }
        std::vector<Tools::RowValueFormatter>& get_row_values()
        {
            auto& var = row_values_;
            return var;
        }
        ConditionalField& get_conditional_field()
        {
            auto& var = conditional_field_;
            return var;
        }

        void set_type(ConditionType type) { type_ = type; }

        bool VerifyCondition_(std::shared_ptr<Results>& results);

    private:
        ConditionType type_;
        Tools::RowValueFormatter row_value_;
        std::vector<Tools::RowValueFormatter> row_values_;
        ConditionalField conditional_field_;
};

#endif // CPW_QUERY_CONDITION_H
