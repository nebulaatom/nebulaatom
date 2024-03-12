/*
* CPW Woodpecker Server
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

#include "filters/fields_filter.h"

using namespace AtomFilters;

FieldsFilterElement::FieldsFilterElement(std::string field) :
    field_(field)
    ,as_("")
    ,type_(Type::kNoQuotes)
{
    AddTypes_();
}

FieldsFilterElement::FieldsFilterElement(std::string field, std::string as, std::string type) :
    field_(field)
    ,as_(as)
    ,type_(Type::kNoQuotes)
{
    AddTypes_();

    auto found = types_.find(type);
    if(found != types_.end())
        type_ = types_[type];
}

void FieldsFilterElement::AddTypes_()
{
    types_.insert(std::make_pair("quotes", Type::kQuotes));
    types_.insert(std::make_pair("no-quotes", Type::kNoQuotes));
}

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

    // Verify contents array
    if(filter_json->get("contents").isEmpty() || !filter_json->get("contents").isArray())
        throw std::runtime_error("contents in kFields is wrong");

    // Iterate over contents array
    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        // Verify array element
        if(!contents_array->isObject(a))
            throw std::runtime_error("contents_array[" + std::to_string(a) + "] is not an object in kfields");

        // Verify array element "field"
        auto content_element = contents_array->getObject(a);
        if(content_element->get("field").isEmpty())
            continue;

        auto field = content_element->get("field").toString();

        // Verify array element "as"
        std::string as = "";
        if(!content_element->get("as").isEmpty())
            as = content_element->get("as").toString();

        // Verify array element "type"
        std::string type = "";
        if(!content_element->get("type").isEmpty())
            type = content_element->get("type").toString();

        // Add element
        filter_elements_.push_back({field, as, type});
    }
}

void FieldsFilter::Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters)
{
    if(filter_elements_.size() < 1)
        return;

    for(auto it = filter_elements_.begin(); it != filter_elements_.end(); it++)
    {
        if(it != filter_elements_.begin())
            tmp_query.push_back(",");

        auto field = Extras::ValuesProperties{it->get_field(), false};
        auto as = Extras::ValuesProperties{it->get_as(), true};
        switch(it->get_type())
        {
            case FieldsFilterElement::Type::kQuotes: field.set_quotes(true); break;
            case FieldsFilterElement::Type::kNoQuotes: field.set_quotes(false); break;
        }
        tmp_query.push_back(field.GetFinalValue());

        if(it->get_as() != "")
        {
            tmp_query.push_back("AS");
            tmp_query.push_back(as.GetFinalValue());
        }
    }
}
