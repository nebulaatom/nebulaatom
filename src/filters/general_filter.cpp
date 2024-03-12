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

#include "filters/general_filter.h"

using namespace AtomFilters;

GeneralFilterElement::GeneralFilterElement(Tools::RowValueFormatter value, Type type) :
    value_(value)
    ,type_(type)
    ,editable_(false)
{

}

GeneralFilter::GeneralFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kGeneral;
}

GeneralFilter::~GeneralFilter()
{

}

void GeneralFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("content in GeneralFilter::Identify_() is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in GeneralFilter::Identify_()");

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

void GeneralFilter::Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters)
{
    /*if(std::stoi(filter_elements_.get_limit()) > 0)
    {
        tmp_query.push_back("LIMIT");
        if(filter_elements_.get_pagination())
        {
            int offset = std::stoi(filter_elements_.get_limit()) * std::stoi(filter_elements_.get_page());
            tmp_query.push_back(std::to_string(offset));
            tmp_query.push_back(", " + filter_elements_.get_limit());
        }
        else
            tmp_query.push_back(filter_elements_.get_limit());
    }
    else
        if(filter_elements_.get_pagination())
            tmp_query.push_back("LIMIT 0, 20");
        else
            tmp_query.push_back("LIMIT 20");*/
}

void GeneralFilter::IncorporateSelected_(VectorString& tmp_query, GeneralFilterElement::Type type)
{
    auto found = std::find_if(filter_elements_.begin(), filter_elements_.end(), [type](const std::pair<std::string, Filters::GeneralFilterElement>& pair)
    {
        return type == pair.second.get_type();
    });
    if(found == filter_elements_.end())
        return;

    switch(type)
    {
        case GeneralFilterElement::Type::kLimit:
        {
            tmp_query.push_back("LIMIT");
            if(found->second.get_value().get_value_int() == 0)
                tmp_query.push_back("20");
            else
            {
                auto limit = found->second.get_value().get_value_int();
                tmp_query.push_back(std::to_string(limit));
            }

            break;
        }
        case GeneralFilterElement::Type::kPageLimit:
        {
            tmp_query.push_back("LIMIT");
            if(found->second.get_value().get_value_string() == "")
                tmp_query.push_back("0, 20");
            else
                tmp_query.push_back(found->second.get_value().get_value_string());

            break;
        }
        case GeneralFilterElement::Type::kAs:
        {
            std::cout << "AS: " << found->second.get_value().get_value_string() << std::endl;
            if(found->second.get_value().get_value() != nullptr)
                std::cout << "AS2: " << found->second.get_value().get_value()->toString() << std::endl;
            if(found->second.get_value().get_value_string() == "")
                return;

            tmp_query.push_back("AS");
            tmp_query.push_back(found->second.get_value().get_value_string());

            break;
        }
    }
}

void GeneralFilter::ReplaceFilterElement(std::string id, Dynamic::Var& value)
{
    auto found = filter_elements_.find(id);
    if(found == filter_elements_.end())
        return;

    if(!found->second.get_editable())
        return;

    Tools::RowValueFormatter element_value = {value};
    found->second = GeneralFilterElement{element_value, found->second.get_type()};
}
