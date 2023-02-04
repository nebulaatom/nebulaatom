/*
* <one line to give the program's name and a brief idea of what it does.>
* Copyright (C) 2021  <copyright holder> <email>
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

#ifndef CPW_FILTERS_FILTERS_H
#define CPW_FILTERS_FILTERS_H


#include <string>
#include <map>
#include <vector>
#include <functional>

#include "extras/values_properties.h"

namespace CPW
{
    namespace Filters
    {
        enum class FilterType;
        class Filters;
    }
}


using namespace CPW::Extras;


enum class CPW::Filters::FilterType
{
    kFields
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


class CPW::Filters::Filters
{
    public:
        using ValuesPropertiesVector = std::vector<ValuesProperties>;
        using MapForValues = std::map<std::string, ValuesProperties>;

        Filters();
        ~Filters();

        ValuesPropertiesVector& get_fields()
        {
            auto& var = fields_;
            return var;
        }
        std::string get_page() const { return page_; }
        std::string get_limit() const { return limit_; }
        ValuesPropertiesVector& get_sorts_conditions()
        {
            auto& var = sorts_conditions_;
            return var;
        }
        MapForValues& get_iquals_conditions()
        {
            auto& var = iquals_conditions_;
            return var;
        }
        MapForValues& get_not_iquals_conditions()
        {
            auto& var = not_iquals_conditions_;
            return var;
        }
        MapForValues& get_greather_than()
        {
            auto& var = greather_than_;
            return var;
        }
        MapForValues& get_smaller_than()
        {
            auto& var = smaller_than_;
            return var;
        }
        std::map<std::string, std::pair<ValuesProperties, ValuesProperties>>& get_between()
        {
            auto& var = between_;
            return var;
        }
        std::map<std::string, ValuesPropertiesVector>& get_in()
        {
            auto& var = in_;
            return var;
        }
        std::map<std::string, ValuesPropertiesVector>& get_not_in()
        {
            auto& var = not_in_;
            return var;
        }
        std::vector<ValuesPropertiesVector>& get_values()
        {
            auto& var = values_;
            return var;
        }
        MapForValues& get_set()
        {
            auto& var = set_;
            return var;
        }
        std::vector<std::pair<std::array<std::string, 2>, MapForValues>>& get_joins()
        {
            auto& var = joins_;
            return var;
        }
        MapForValues& get_like()
        {
            auto& var = like_;
            return var;
        }
        std::map<std::string, FilterType>& get_filters_type_map()
        {
            auto& var = filters_type_map_;
            return var;
        }
        std::string& get_as()
        {
            auto& var = as_;
            return var;
        }
        ValuesPropertiesVector& get_group_conditions()
        {
            auto& var = group_conditions_;
            return var;
        }

        void set_page(std::string page) { page_ = page; }
        void set_limit(std::string limit) { limit_ = limit; }
        void set_as(std::string as) { as_ = as; }

        bool ExistsType_(std::string type);

    private:
        void MapFilterType_();

        ValuesPropertiesVector fields_;
        std::string page_;
        std::string limit_;
        ValuesPropertiesVector sorts_conditions_;
        MapForValues iquals_conditions_;
        MapForValues not_iquals_conditions_;
        MapForValues greather_than_;
        MapForValues smaller_than_;
        std::map<std::string, std::pair<ValuesProperties, ValuesProperties>> between_;
        std::map<std::string, ValuesPropertiesVector> in_;
        std::map<std::string, ValuesPropertiesVector> not_in_;
        std::vector<ValuesPropertiesVector> values_;
        MapForValues set_;
        std::vector<std::pair<std::array<std::string, 2>, MapForValues>> joins_;
        MapForValues like_;
        std::map<std::string, FilterType> filters_type_map_;
        std::string as_;
        ValuesPropertiesVector group_conditions_;
};


#endif // CPW_FILTERS_FILTERS_H
