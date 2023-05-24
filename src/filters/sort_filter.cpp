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

#include "filters/sort_filter.h"

using namespace CPW::Filters;

SortFilter::SortFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kSort;
}

SortFilter::~SortFilter()
{

}

void SortFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
    throw std::runtime_error("contents in kSort is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in kSort");

        // Verify array element "value"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("value").isEmpty())
            continue;

        std::string value = content_element->get("value").toString();

        // Verify array element "order"
        std::string order = "";
        if(!content_element->get("order").isEmpty())
            order = content_element->get("order").toString();

        // Add element
        Add_(value, order);
    }
}

void SortFilter::Incorporate_(VectorString& tmp_query)
{
    if(sort_conditions_.size() > 0)
    {
        tmp_query.push_back("ORDER BY");
        for(auto it = sort_conditions_.begin(); it != sort_conditions_.end(); it++)
        {
            if(it != sort_conditions_.begin())
                tmp_query.push_back(",");

            tmp_query.push_back(it->value_.GetFinalValue());
            tmp_query.push_back(it->order_);
        }
    }
}

void SortFilter::Add_(std::string value, std::string order)
{
    Filters::SortCondition sort_condition
    {
        Extras::ValuesProperties{value, false}
        ,order
    };
    sort_conditions_.push_back(std::move(sort_condition));
}
