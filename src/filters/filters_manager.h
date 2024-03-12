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

#ifndef ATOMFILTERS_FILTERS_MANAGER_H
#define ATOMFILTERS_FILTERS_MANAGER_H


#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>

#include "filters/fields_filter.h"
#include "filters/sort_filter.h"
#include "filters/general_filter.h"
#include "filters/iquals_filter.h"
#include "filters/range_filter.h"
#include "filters/list_filter.h"
#include "filters/like_filter.h"
#include "filters/join_filter.h"
#include "filters/group_filter.h"
#include "filters/values_filter.h"
#include "filters/set_filter.h"

namespace CPW
{
    namespace Filters
    {
        class FiltersManager;
    }
}


using namespace AtomExtras;


class AtomFilters::FiltersManager
{
    public:
        using ValuesPropertiesVector = std::vector<ValuesProperties>;
        using MapForValues = std::map<std::string, ValuesProperties>;

        FiltersManager();
        ~FiltersManager();

        std::shared_ptr<Filters::FieldsFilter>& get_fields_filter()
        {
            auto& var = fields_;
            return var;
        }
        std::shared_ptr<Filters::SortFilter>& get_sort_filter()
        {
            auto& var = sort_filter_;
            return var;
        }
        std::shared_ptr<Filters::GeneralFilter>& get_general_filter()
        {
            auto& var = general_filter_;
            return var;
        }
        std::shared_ptr<Filters::IqualsFilter>& get_iquals_filter()
        {
            auto& var = iquals_filter_;
            return var;
        }
        std::shared_ptr<Filters::RangeFilter>& get_range_filter()
        {
            auto& var = range_filter_;
            return var;
        }
        std::shared_ptr<Filters::ListFilter>& get_list_filter()
        {
            auto& var = list_filter_;
            return var;
        }
        std::shared_ptr<Filters::LikeFilter>& get_like_filter()
        {
            auto& var = like_filter_;
            return var;
        }
        std::shared_ptr<Filters::JoinFilter>& get_join_filter()
        {
            auto& var = join_filter_;
            return var;
        }
        std::shared_ptr<Filters::GroupFilter>& get_group_filter()
        {
            auto& var = group_filter_;
            return var;
        }
        std::shared_ptr<Filters::ValuesFilter>& get_values_filter()
        {
            auto& var = values_filter_;
            return var;
        }
        std::shared_ptr<Filters::SetFilter>& get_set_filter()
        {
            auto& var = set_filter_;
            return var;
        }
        std::map<std::string, FilterType>& get_filters_type_map()
        {
            auto& var = filters_type_map_;
            return var;
        }
        bool ExistsType_(std::string type);

    private:
        void MapFilterType_();

        std::shared_ptr<Filters::FieldsFilter> fields_;
        std::shared_ptr<Filters::SortFilter> sort_filter_;
        std::shared_ptr<Filters::GeneralFilter> general_filter_;
        std::shared_ptr<Filters::IqualsFilter> iquals_filter_;
        std::shared_ptr<Filters::RangeFilter> range_filter_;
        std::shared_ptr<Filters::ListFilter> list_filter_;
        std::shared_ptr<Filters::LikeFilter> like_filter_;
        std::shared_ptr<Filters::JoinFilter> join_filter_;
        std::shared_ptr<Filters::GroupFilter> group_filter_;
        std::shared_ptr<Filters::ValuesFilter> values_filter_;
        std::shared_ptr<Filters::SetFilter> set_filter_;
        std::map<std::string, FilterType> filters_type_map_;
};


#endif // ATOMFILTERS_FILTERS_MANAGER_H
