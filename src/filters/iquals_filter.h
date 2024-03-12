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

#ifndef ATOMFILTERS_IQUALS_FILTER_H
#define ATOMFILTERS_IQUALS_FILTER_H


#include "filters/filter.h"


namespace CPW
{
    namespace Filters
    {
        class IqualsFilterElement;
        class IqualsFilter;
    }
}


class AtomFilters::IqualsFilterElement
{
    public:
        enum class Type
        {
            kIqual
            ,kNoIqual
        };

        IqualsFilterElement(std::string col, Tools::RowValueFormatter value, Type type);

        std::string get_col() const { return col_; }
        Tools::RowValueFormatter& get_value()
        {
            auto& var = value_;
            return var;
        }
        Type get_type() const { return type_; }
        bool get_editable() const { return editable_; }

        void set_col(std::string col) { col_ = col; }
        void set_type(Type type) { type_ = type; }
        void set_editable(bool editable) { editable_ = editable; }

    private:
        std::string col_;
        Tools::RowValueFormatter value_;
        Type type_;
        bool editable_;
};

class AtomFilters::IqualsFilter : Filters::Filter
{
    public:
        IqualsFilter();
        virtual ~IqualsFilter();

        std::map<std::string, Filters::IqualsFilterElement>& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters) override;

    protected:
        void ReplaceFilterElement(std::string id, Dynamic::Var& value);

    private:
        std::map<std::string, Filters::IqualsFilterElement> filter_elements_;
};


#endif // ATOMFILTERS_IQUALS_FILTER_H
