/*
* Nebula Atom

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

#include "filters/range_filter.h"

using namespace AtomFilters;

RangeFilterElement::RangeFilterElement(std::string col, Tools::RowValueFormatter value, std::string type) :
    col_(col)
    ,value_(value)
    ,type_(Type::kGreather)
{
    AddTypes_();

    auto found = types_.find(type);
    if(found != types_.end())
        type_ = types_[type];
}

void RangeFilterElement::AddTypes_()
{
    types_.insert(std::make_pair("greather", Type::kGreather));
    types_.insert(std::make_pair("greather-iqual", Type::kGreatherIqual));
    types_.insert(std::make_pair("smaller", Type::kSmaller));
    types_.insert(std::make_pair("smaller-iqual", Type::kSmallerIqual));
}

RangeFilter::RangeFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kRange;
}

RangeFilter::~RangeFilter()
{

}

void RangeFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("content in RangeFilter::Identify_() is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in RangeFilter::Identify_()");

        // Verify array element "col"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("col").isEmpty())
            continue;

        std::string col = content_element->get("col").toString();

        // Verify array element "value"
        auto var_value = content_element->get("value");
        auto value = Tools::RowValueFormatter(var_value);

        // Verify array element "type"
        std::string type = "";
        if(!content_element->get("type").isEmpty())
            type = content_element->get("type").toString();

        // Add element
        filter_elements_.push_back(RangeFilterElement{col, value, type});
    }
}

void RangeFilter::Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters)
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
                case RangeFilterElement::Type::kGreather:
                    tmp_query.push_back(">");
                    break;
                case RangeFilterElement::Type::kGreatherIqual:
                    tmp_query.push_back(">=");
                    break;
                case RangeFilterElement::Type::kSmaller:
                    tmp_query.push_back("<");
                    break;
                case RangeFilterElement::Type::kSmallerIqual:
                    tmp_query.push_back("<=");
                    break;
            }
            tmp_query.push_back("?");

            query_parameters.push_back(it->get_value());
        }
    }
}
