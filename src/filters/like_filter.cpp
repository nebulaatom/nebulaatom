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

#include "filters/like_filter.h"

using namespace AtomFilters;

LikeFilterElement::LikeFilterElement(std::string col, Tools::RowValueFormatter value, std::string type) :
    col_(col)
    ,value_(value)
    ,type_(Type::kPerLikePer)
{
    AddTypes_();

    auto found = types_.find(type);
    if(found != types_.end())
        type_ = types_[type];
}

void LikeFilterElement::AddTypes_()
{
    types_.insert(std::make_pair("%like", Type::kPerLike));
    types_.insert(std::make_pair("like%", Type::kLikePer));
    types_.insert(std::make_pair("%like%", Type::kPerLikePer));
    types_.insert(std::make_pair("_like", Type::kUndLike));
    types_.insert(std::make_pair("like_", Type::kLikeUnd));
    types_.insert(std::make_pair("_like_", Type::kUndLikeUnd));
}

LikeFilter::LikeFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kLike;
}

LikeFilter::~LikeFilter()
{

}

void LikeFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("content in LikeFilter::Identify_() is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in LikeFilter::Identify_()");

        // Verify array element "col"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("col").isEmpty())
            continue;

        std::string col = content_element->get("col").toString();

        // Get array element "value"
        auto var_value = content_element->get("value");
        auto value = Tools::RowValueFormatter(var_value);

        // Verify array element "type"
        std::string type = "";
        if(!content_element->get("type").isEmpty())
            type = content_element->get("type").toString();

        // Add element
        filter_elements_.push_back({col, value, type});
    }
}

void LikeFilter::Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters)
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

        tmp_query.push_back(it->get_col());
        tmp_query.push_back("LIKE");
        tmp_query.push_back("?");

        auto& value_string = it->get_value().get_value_string();
        switch(it->get_type())
        {
            case LikeFilterElement::Type::kPerLike: value_string = "%" + value_string; break;
            case LikeFilterElement::Type::kLikePer: value_string = value_string + "%"; break;
            case LikeFilterElement::Type::kPerLikePer: value_string = "%" + value_string + "%"; break;
            case LikeFilterElement::Type::kUndLike: value_string = "_" + value_string; break;
            case LikeFilterElement::Type::kLikeUnd: value_string = value_string + "_"; break;
            case LikeFilterElement::Type::kUndLikeUnd: value_string = "_" + value_string + "_"; break;
        }
        query_parameters.push_back(it->get_value());
    }
}
