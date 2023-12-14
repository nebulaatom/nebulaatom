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

#ifndef CPW_QUERY_PARAMETER_H
#define CPW_QUERY_PARAMETER_H


#include <string>

#include "tools/row_value_formatter.h"
#include "query/results.h"


namespace CPW
{
    namespace Query
    {
        enum class ParameterType;
        class Parameter;
    }
}


enum class CPW::Query::ParameterType
{
    kField
    ,kConditional
};

class CPW::Query::Parameter
{
    public:
        Parameter(std::string name, Tools::RowValueFormatter value, bool editable);
        Parameter(std::string name, Query::ConditionalField conditional_field, std::shared_ptr<Query::Results> result, bool editable);

        ParameterType get_parameter_type() const { return parameter_type_; }
        std::string get_name() const { return name_; }
        bool get_editable() const { return editable_; }
        Tools::RowValueFormatter& get_value()
        {
            auto& var = value_;
            return var;
        }
        Query::ConditionalField& get_conditional_field()
        {
            auto& var = conditional_field_;
            return var;
        }
        std::shared_ptr<Query::Results>& get_result()
        {
            auto& var = result_;
            return var;
        }

        void set_parameter_type(ParameterType parameter_type) { parameter_type_ = parameter_type; }
        void set_name(std::string name) { name_ = name; }
        void set_editable(bool editable) { editable_ = editable; }

    private:
        ParameterType parameter_type_;
        std::string name_;
        bool editable_;
        Tools::RowValueFormatter value_;
        Query::ConditionalField conditional_field_;
        std::shared_ptr<Query::Results> result_;
};

#endif // CPW_QUERY_PARAMETER_H
