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

#include "filters/set_filter.h"

using namespace CPW::Filters;

SetFilterElement::SetFilterElement(std::string col, Tools::RowValueFormatter value) :
    col_(col)
    ,value_(value)
{

}

SetFilter::SetFilter()
{

}

SetFilter::~SetFilter()
{

}

void SetFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("content in SetFilter::Identify_() is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in SetFilter::Identify_()");

        // Verify array element "col"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("col").isEmpty())
            continue;

        std::string col = content_element->get("col").toString();

        // Get array element "value"
        auto var_value = content_element->get("value");
        auto value = Tools::RowValueFormatter(var_value);

        // Add element
        filter_elements_.push_back({col, value});
    }
}

void SetFilter::Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters)
{
    if(filter_elements_.size() < 1)
        return;

    tmp_query.push_back("SET");
    for(auto it = filter_elements_.begin(); it != filter_elements_.end(); it++)
    {
        if(it != filter_elements_.begin())
            tmp_query.push_back(",");

        tmp_query.push_back(it->get_col());
        tmp_query.push_back("=");
        tmp_query.push_back("?");

        query_parameters.push_back(it->get_value());
    }
}
