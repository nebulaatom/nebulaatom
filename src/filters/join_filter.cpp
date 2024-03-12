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

#include "filters/join_filter.h"

using namespace AtomFilters;

JoinFilterElement::JoinFilterElement(std::string table, std::string as, std::list<OnElement> on, std::string type) :
    table_(table)
    ,as_(as)
    ,on_(on)
    ,type_(Type::kInner)
{
    AddTypes_();

    auto found = types_.find(type);
    if(found != types_.end())
        type_ = types_[type];
}

void JoinFilterElement::AddTypes_()
{
    types_.insert(std::make_pair("inner", Type::kInner));
    types_.insert(std::make_pair("left", Type::kLeft));
    types_.insert(std::make_pair("right", Type::kRight));
}

JoinFilter::JoinFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kJoin;
}

JoinFilter::~JoinFilter()
{

}

void JoinFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("content in JoinFilter::Identify_() is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in JoinFilter::Identify_()");

        // Verify array element "table"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("table").isEmpty())
            continue;

        std::string table = content_element->get("table").toString();

        // Verify array element "as"
        if(content_element->get("as").isEmpty())
            continue;

        std::string as = content_element->get("as").toString();

        // Verify array elements "on"
        std::list<JoinFilterElement::OnElement> on;
        auto values_array = content_element->getArray("on");
        for(std::size_t b = 0; b < values_array->size(); b++)
        {
            // Verify array element
            if(!values_array->isObject(b))
                continue;

            auto on_element = values_array->getObject(b);

            // Verify "col" element
            if(on_element->get("col").isEmpty())
                continue;

            auto col = on_element->get("col").toString();

            // Verify "value" and "value-quoted" element
            auto value = Extras::ValuesProperties{"", false};
            if(on_element->get("value").isEmpty())
                continue;
            else if(!on_element->get("value-quoted").isEmpty())
                value.set_quotes(true);

            value.set_value(on_element->get("value").toString());

            on.push_back({col, value});
        }

        // Verify array element "type"
        std::string type = "";
        if(!content_element->get("type").isEmpty())
            type = content_element->get("type").toString();

        // Add element
        filter_elements_.push_back({table, as, on, type});
    }
}

void JoinFilter::Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters)
{
    if(filter_elements_.size() < 1)
        return;

    for(auto it = filter_elements_.begin(); it != filter_elements_.end(); it++)
    {
        switch(it->get_type())
        {
            case JoinFilterElement::Type::kInner: tmp_query.push_back("INNER JOIN"); break;
            case JoinFilterElement::Type::kLeft: tmp_query.push_back("LEFT JOIN"); break;
            case JoinFilterElement::Type::kRight: tmp_query.push_back("RIGHT JOIN"); break;
        }
        tmp_query.push_back(it->get_table());
        tmp_query.push_back(it->get_as());
        tmp_query.push_back("ON");

        for(auto it2 = it->get_on().begin(); it2 != it->get_on().end(); it2++)
        {
            if(it2 != it->get_on().begin())
                tmp_query.push_back(",");

            tmp_query.push_back(it2->col);
            tmp_query.push_back("=");
            tmp_query.push_back(it2->value.GetFinalValue());
        }
    }
}
