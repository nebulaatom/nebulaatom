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

#ifndef CPW_FILTERS_FILTER_H
#define CPW_FILTERS_FILTER_H


#include <string>
#include <map>
#include <vector>
#include <list>
#include <forward_list>
#include <functional>


#include "extras/values_properties.h"


namespace CPW
{
    namespace Filters
    {
        enum class FilterType;
        class Filter;
    }
}

using namespace CPW::Extras;


enum class CPW::Filters::FilterType
{
    kUnknown
    ,kFields
    ,kPage
    ,kLimit
    ,kSort
    ,kIqual
    ,kNotIqual
    ,kGreatherThan
    ,kSmallerThan
    ,kBetween
    ,kIn
    ,kNotIn
    ,kValues
    ,kSet
    ,kJoins
    ,kLike
    ,kAS
    ,kGroup
};


class CPW::Filters::Filter
{
    public:
        using ValuesPropertiesVector = std::vector<ValuesProperties>;
        using MapForValues = std::map<std::string, ValuesProperties>;

        Filter();
        virtual ~Filter();

        Filters::FilterType& get_current_filter_type()
        {
            auto& var = current_filter_type_;
            return var;
        }

    protected:

    private:
        Filters::FilterType current_filter_type_;
};


#endif // CPW_FILTERS_FILTER_H
