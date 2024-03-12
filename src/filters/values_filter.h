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

#ifndef ATOMFILTERS_VALUES_FILTER_H
#define ATOMFILTERS_VALUES_FILTER_H


#include "filters/filter.h"


namespace CPW
{
    namespace Filters
    {
        class ValuesFilterElement;
        class ValuesFilter;
    }
}


class AtomFilters::ValuesFilterElement
{
    public:
        ValuesFilterElement(Tools::RowValueFormatter value);

        Tools::RowValueFormatter& get_value()
        {
            auto& var = value_;
            return var;
        }

    private:
        Tools::RowValueFormatter value_;
};

class AtomFilters::ValuesFilter : Filters::Filter
{
    public:
        ValuesFilter();
        virtual ~ValuesFilter();

        std::list<Filters::ValuesFilterElement>& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters) override;

    private:
        std::list<Filters::ValuesFilterElement> filter_elements_;
};


#endif // ATOMFILTERS_VALUES_FILTER_H
