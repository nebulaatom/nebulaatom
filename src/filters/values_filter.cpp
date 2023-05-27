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

#include "filters/values_filter.h"

using namespace CPW::Filters;

ValuesFilterElement::ValuesFilterElement(std::string value, std::string type) :
    value_(value)
    ,type_(Type::kQuotes)
{
    AddTypes_();

    auto found = types_.find(type);
    if(found != types_.end())
        type_ = types_[type];
}

void ValuesFilterElement::AddTypes_()
{
    types_.insert(std::make_pair("quotes", Type::kQuotes));
    types_.insert(std::make_pair("no-quotes", Type::kNoQuotes));
}

ValuesFilter::ValuesFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kValues;
}

ValuesFilter::~ValuesFilter()
{

}

void ValuesFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("content in ValuesFilter::Identify_() is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in ValuesFilter::Identify_()");

        // Verify array element "value"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("value").isEmpty())
            continue;

        std::string value = content_element->get("value").toString();

        // Verify array element "type"
        std::string type = "";
        if(!content_element->get("type").isEmpty())
            type = content_element->get("type").toString();

        // Add element
        filter_elements_.push_back({value, type});
    }
}

void ValuesFilter::Incorporate_(VectorString& tmp_query)
{
    if(filter_elements_.size() < 1)
        return;

    tmp_query.push_back("VALUES (");
    for(auto it = filter_elements_.begin(); it != filter_elements_.end(); it++)
    {
        if(it != filter_elements_.begin())
            tmp_query.push_back(",");

        Extras::ValuesProperties value {it->get_value(), true};
        switch(it->get_type())
        {
            case ValuesFilterElement::Type::kQuotes:
                tmp_query.push_back(value.GetFinalValue());
                break;
            case ValuesFilterElement::Type::kNoQuotes:
                tmp_query.push_back(it->get_value());
                break;
        }
    }
    tmp_query.push_back(")");
}
