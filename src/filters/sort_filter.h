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

#ifndef CPW_FILTERS_SORT_FILTER_H
#define CPW_FILTERS_SORT_FILTER_H


#include "filters/filter.h"


namespace CPW
{
    namespace Filters
    {
        class SortCondition;
        class SortFilter;
    }
}


class CPW::Filters::SortCondition
{
    public:
        SortCondition(Extras::ValuesProperties value, std::string order = "") :
            value_(value)
            ,order_(order)
        {

        }
        SortCondition() :
            value_("", true)
            ,order_("")
        {

        }

        Extras::ValuesProperties value_;
        std::string order_;
};

class CPW::Filters::SortFilter : Filters::Filter
{
    public:
        SortFilter();
        virtual ~SortFilter();

        std::list<SortCondition>& get_sort_conditions()
        {
            auto& var = sort_conditions_;
            return var;
        }
        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query) override;

    protected:
        void Add_(std::string value, std::string order);

    private:
        std::list<SortCondition> sort_conditions_;
};


#endif // CPW_FILTERS_SORT_FILTER_H
