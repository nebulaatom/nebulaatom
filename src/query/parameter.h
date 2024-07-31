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

#ifndef ATOM_QUERY_PARAMETER
#define ATOM_QUERY_PARAMETER


#include <string>

#include "tools/dvalue.h"
#include "query/results.h"
#include "query/condition.h"


namespace Atom
{
    namespace Query
    {
        enum class ParameterType;
        class Parameter;
    }
}


enum class Atom::Query::ParameterType
{
    kField
    ,kPosition
};

class Atom::Query::Parameter
{
    public:
        using Ptr = std::shared_ptr<Parameter>;

        Parameter(std::string name, Tools::DValue value, bool editable);
        Parameter(std::string name, Query::Field::Position field_position, std::string related_action, bool editable);

        ParameterType get_parameter_type() const { return parameter_type_; }
        std::string get_name() const { return name_; }
        std::string get_error() const { return error_; }
        bool get_editable() const { return editable_; }
        Tools::DValue& get_value()
        {
            auto& var = value_;
            return var;
        }
        Query::Field::Position& get_field_position()
        {
            auto& var = field_position_;
            return var;
        }
        std::string get_related_action() const { return related_action_; }
        Query::Condition<Ptr>::Ptr& get_condition()
        {
            auto& var = condition_;
            return var;
        }

        void set_parameter_type(ParameterType parameter_type) { parameter_type_ = parameter_type; }
        void set_name(std::string name) { name_ = name; }
        void set_error(std::string error) { error_ = error; }
        void set_editable(bool editable) { editable_ = editable; }
        void set_value(Tools::DValue value) { value_ = value; }
        void set_related_action(std::string related_action) { related_action_ = related_action; }

        std::string ToString_();
        std::string StringValue_();
        int IntValue_();
        bool BoolValue_();
        float FloatValue_();
        void SetupCondition_(std::string identifier, Query::ConditionType type, Query::Condition<Ptr>::Functor functor);

    private:
        ParameterType parameter_type_;
        std::string name_;
        std::string error_;
        bool editable_;
        Tools::DValue value_;
        Query::Field::Position field_position_;
        std::string related_action_;
        Query::Condition<Ptr>::Ptr condition_;
};

#endif // ATOM_QUERY_PARAMETER
