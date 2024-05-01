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

#ifndef ATOM_QUERY_CONDITION
#define ATOM_QUERY_CONDITION


#include "query/results.h"
#include "tools/output_logger.h"


namespace Atom
{
    namespace Query
    {
        enum class ConditionType;
        class Condition;
    }
}


enum class Atom::Query::ConditionType
{
    kWarning
    ,kError
};

class Atom::Query::Condition
{
    public:
        using Ptr = std::shared_ptr<Condition>;
        using Functor = std::function<bool(Results::Ptr)>;

        Condition(std::string identifier, ConditionType type, Functor functor);

        ConditionType get_type() const { return type_; }
        std::string get_identifier() const { return identifier_; }
        Functor& get_functor()
        {
            auto& var = functor_;
            return var;
        }

        void set_identifier(std::string identifier) { identifier_ = identifier; }
        void set_type(ConditionType type) { type_ = type; }

        bool VerifyCondition_(Results::Ptr results);

    private:
        std::string identifier_;
        ConditionType type_;
        Functor functor_;
};

#endif // ATOM_QUERY_CONDITION
