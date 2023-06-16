/*
* CPW Woodpecker Server
* Copyright (C) 2021 CPW Online support@cpwonline.org
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

#ifndef CPW_FILTERS_JOIN_FILTER_H
#define CPW_FILTERS_JOIN_FILTER_H


#include "filters/filter.h"


namespace CPW
{
    namespace Filters
    {
        class JoinFilterElement;
        class JoinFilter;
    }
}


class CPW::Filters::JoinFilterElement
{
    public:
        enum class Type
        {
            kInner
            ,kLeft
            ,kRight
        };

        struct OnElement
        {
            OnElement(std::string col, Extras::ValuesProperties value) : col(col), value(value) {}

            std::string col;
            Extras::ValuesProperties value;
        };

        JoinFilterElement(std::string table, std::string as, std::list<OnElement> on, std::string type);

        std::string get_table() const { return table_; }
        std::string get_as() const { return as_; }
        std::list<OnElement>& get_on()
        {
            auto& var = on_;
            return var;
        }
        Type get_type() const { return type_; }
        std::map<std::string, Type>& get_types()
        {
            auto& var = types_;
            return var;
        }

        void set_table(std::string table) { table_ = table; }
        void set_as(std::string as) { as_ = as; }
        void set_type(Type type) { type_ = type; }

    protected:
        void AddTypes_();

    private:
        std::string table_;
        std::string as_;
        std::list<OnElement> on_;
        Type type_;
        std::map<std::string, Type> types_;
};

class CPW::Filters::JoinFilter : Filters::Filter
{
    public:
        JoinFilter();
        virtual ~JoinFilter();

        std::list<Filters::JoinFilterElement>& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters) override;

    private:
        std::list<Filters::JoinFilterElement> filter_elements_;
};


#endif // CPW_FILTERS_JOIN_FILTER_H
