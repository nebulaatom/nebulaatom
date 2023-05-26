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

        // Verify array element "col"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("col").isEmpty())
            continue;

        std::string col = content_element->get("col").toString();

        // Verify array element "value"
        if(content_element->get("value").isEmpty())
            continue;

        auto var_value = content_element->get("value");
        auto value = GetValueProperties_(var_value);

        // Verify array element "not"
        bool not_iqual = false;
        if(!content_element->get("not").isEmpty())
        {
            auto var_not = content_element->get("not");
            Tools::RowValueFormatter row(var_not);
            row.Format_();
            not_iqual = row.get_value_bool();
        }

        // Add element
        filter_elements_.iqual_conditions_.push_back(IqualsFilterElements::IqualCondition{col, value, not_iqual});
    }
}

void IqualsFilter::Incorporate_(VectorString& tmp_query)
{
    auto& iqual_conditions = filter_elements_.iqual_conditions_;
    if(iqual_conditions.size() > 0)
    {
        if(!FindWHERE_(tmp_query))
            tmp_query.push_back("WHERE");

        for(auto it = iqual_conditions.begin(); it != iqual_conditions.end(); it++)
        {
            if(it == iqual_conditions.begin())
            {
                if(FindAND_(tmp_query))
                    tmp_query.push_back("AND");
            }
            else
                tmp_query.push_back("AND");

            tmp_query.push_back(it->col);
            tmp_query.push_back("=");
            tmp_query.push_back(it->value.GetFinalValue());
        }
    }
}
