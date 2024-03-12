/*
* Nebula Atom
* Copyright (C) 2021 NebulaCSF support@cpwonline.org
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

#include "filters/list_filter.h"

using namespace AtomFilters;

ListFilterElement::ListFilterElement(std::string col, std::list<Tools::RowValueFormatter> values, std::string type) :
    col_(col)
    ,values_(values)
    ,type_(Type::kIn)
{
    AddTypes_();

    auto found = types_.find(type);
    if(found != types_.end())
        type_ = types_[type];
}

void ListFilterElement::AddTypes_()
{
    types_.insert(std::make_pair("in", Type::kIn));
    types_.insert(std::make_pair("not-in", Type::kNotIn));
}

ListFilter::ListFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kList;
}

ListFilter::~ListFilter()
{

}

void ListFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("content in ListFilter::Identify_() is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in ListFilter::Identify_()");

        // Verify array element "col"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("col").isEmpty())
            continue;

        std::string col = content_element->get("col").toString();

        // Verify array elements "values"
        std::list<Tools::RowValueFormatter> values;
        auto values_array = content_element->getArray("values");
        for(std::size_t b = 0; b < values_array->size(); b++)
        {
            // Verify object
            if(values_array->isObject(b))
            {
                auto value_object = values_array->getObject(b);
                if(value_object->get("value").isEmpty() || value_object->get("quotes").isEmpty())
                    continue;

                auto value_var = value_object->get("value");
                auto value = Tools::RowValueFormatter(value_var);
                if(value_object->get("quotes").toString() == "y")
                    value.get_value_string() = "'" + value.get_value_string() + "'";

                values.push_back(std::move(value));
            }
            else
            {
                auto value_var = values_array->get(b);
                auto value = Tools::RowValueFormatter(value_var);
                values.push_back(std::move(value));
            }
        }

        // Verify array element "type"
        std::string type = "";
        if(!content_element->get("type").isEmpty())
            type = content_element->get("type").toString();

        // Add element
        filter_elements_.push_back({col, values, type});
    }
}

void ListFilter::Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters)
{
    if(filter_elements_.size() > 0)
    {
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

            tmp_query.push_back(it->get_col());

            switch(it->get_type())
            {
                case ListFilterElement::Type::kIn: tmp_query.push_back("IN ("); break;
                case ListFilterElement::Type::kNotIn: tmp_query.push_back("NOT IN ("); break;
            }

            for(auto it2 = it->get_values().begin(); it2 != it->get_values().end(); it2++)
            {
                if(it2 != it->get_values().begin())
                    tmp_query.push_back(",");

                tmp_query.push_back("?");
                query_parameters.push_back(*it2);
            }
            tmp_query.push_back(")");
        }
    }
}
