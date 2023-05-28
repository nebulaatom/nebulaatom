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

SortFilterElement::SortFilterElement(std::string col, std::string type) :
    col_(col)
    ,type_(Type::kAsc)
{
    AddTypes_();

    auto found = types_.find(type);
    if(found != types_.end())
        type_ = types_[type];
}

void SortFilterElement::AddTypes_()
{
    types_.insert(std::make_pair("asc", Type::kAsc));
    types_.insert(std::make_pair("desc", Type::kDesc));
}

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

        // Verify array element "col"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("col").isEmpty())
            continue;

        std::string col = content_element->get("col").toString();

        // Verify array element "type"
        std::string type = "";
        if(!content_element->get("type").isEmpty())
            type = content_element->get("type").toString();

        // Add element
        filter_elements_.push_back({col, type});
    }
}

void SortFilter::Incorporate_(VectorString& tmp_query)
{
    if(filter_elements_.size() < 1)
        return;

    tmp_query.push_back("ORDER BY");

    for(auto it = filter_elements_.begin(); it != filter_elements_.end(); it++)
    {
        if(it != filter_elements_.begin())
            tmp_query.push_back(",");

        tmp_query.push_back(it->get_col());

        switch(it->get_type())
        {
            case SortFilterElement::Type::kAsc: tmp_query.push_back("ASC"); break;
            case SortFilterElement::Type::kDesc: tmp_query.push_back("DESC"); break;
        }
    }
}
