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

#include "filters/fields_filter.h"

using namespace CPW::Filters;

FieldsFilter::FieldsFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kFields;
}

FieldsFilter::~FieldsFilter()
{

}

void FieldsFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("contents in kFields is wrong");

    std::map<std::string, Extras::ValuesProperties> tmp_joins;

    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in kfields");

        auto content_element = contents_array->getObject(a);
        if(content_element->get("field").isEmpty())
                continue;

        std::string as = "";
        if(!content_element->get("as").isEmpty())
            as = content_element->get("as").toString();

        Add_
        (
            content_element->get("field").toString()
            ,as
        );
    }
}

void FieldsFilter::Add_(std::string value, std::string as)
{
    Filters::Field field;
    field.value_ = Extras::ValuesProperties{value, false};
    field.as_ = as;
    fields_.push_back(std::move(field));
}
