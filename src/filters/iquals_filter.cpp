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

#include "filters/iquals_filter.h"

using namespace CPW::Filters;

IqualsFilterElement::IqualsFilterElement(std::string col, Tools::RowValueFormatter value, Type type) :
    col_(col)
    ,value_(value)
    ,type_(type)
{

}

IqualsFilter::IqualsFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kIqual;
}

IqualsFilter::~IqualsFilter()
{

}

void IqualsFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("content in IqualsFilter::Identify_() is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in IqualsFilter::Identify_()");

        // Verify array element "id"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("id").isEmpty())
            continue;

        std::string id = content_element->get("id").toString();

        // Get array element "value"
        auto value = content_element->get("value");

        // Add element
        ReplaceFilterElement(id, value);
    }
}

void IqualsFilter::Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters)
{
    if(filter_elements_.size() < 1)
        return;

    if(!FindWHERE_(tmp_query))
        tmp_query.push_back("WHERE");

    for(auto it = filter_elements_.begin(); it != filter_elements_.end(); it++)
    {
        if(it == filter_elements_.begin())
        {
            if(FindAND_(tmp_query))
                tmp_query.push_back("AND");
        }
        else
            tmp_query.push_back("AND");

        tmp_query.push_back(it->second.get_col());

        switch(it->second.get_type())
        {
            case IqualsFilterElement::Type::kIqual: tmp_query.push_back("="); break;
            case IqualsFilterElement::Type::kNoIqual: tmp_query.push_back("!="); break;
        }

        tmp_query.push_back("?");
        query_parameters.push_back(it->second.get_value());
    }
}

void IqualsFilter::ReplaceFilterElement(std::string id, Dynamic::Var& value)
{
    auto found = filter_elements_.find(id);
    if(found == filter_elements_.end())
        return;

    try
    {
        Tools::RowValueFormatter element_value = {value};
        found->second = IqualsFilterElement{found->second.get_col(), element_value, found->second.get_type()};
    }
    catch(std::exception e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}
