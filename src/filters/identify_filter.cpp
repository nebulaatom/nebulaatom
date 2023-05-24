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

#include "filters/identify_filter.h"

using namespace CPW::Filters;

IdentifyFilter::IdentifyFilter(std::shared_ptr<FiltersManager> current_filters) :
    current_filters_(current_filters)
{
    MapFilterTypeFunctors_();
}

IdentifyFilter::~IdentifyFilter()
{

}

void IdentifyFilter::Fields_(Dynamic::Var& filter)
{
    current_filters_->get_fields_filter()->Identify_(filter);
}

void IdentifyFilter::Page_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("content").isEmpty())
        throw std::runtime_error("content in kPage is empty");

    current_filters_->set_page(filter_json->get("content").toString());
}

void IdentifyFilter::Limit_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("content").isEmpty())
        throw std::runtime_error("content in kLimit is empty");

    current_filters_->set_limit(filter_json->get("content").toString());
}

void IdentifyFilter::Sort_(Dynamic::Var& filter)
{
    current_filters_->get_sort_filter()->Identify_(filter);
}

void IdentifyFilter::Iqual_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kIqual is empty");

    auto var = filter_json->get("content");

    current_filters_->get_iquals_conditions().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,GetValue_(var)
    ));
}

void IdentifyFilter::NotIqual_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kNotIqual is empty");

    auto var = filter_json->get("content");

    current_filters_->get_not_iquals_conditions().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,GetValue_(var)
    ));
}

void IdentifyFilter::GreatherThan_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kGreatherThan is empty");

    auto var = filter_json->get("content");

    current_filters_->get_greather_than().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,GetValue_(var)
    ));
}

void IdentifyFilter::SmallerThan_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kSmallerThan is empty");

    auto var = filter_json->get("content");

    current_filters_->get_smaller_than().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,GetValue_(var)
    ));
}

void IdentifyFilter::Between_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kBetween is empty");

    auto var1 = filter_json->get("content1");
    auto var2 = filter_json->get("content2");

    current_filters_->get_between().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,std::make_pair
        (
            GetValue_(var1)
            ,GetValue_(var2)
        )
    ));
}

void IdentifyFilter::In_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kIn is empty");

    std::vector<Extras::ValuesProperties> tmp_in;
    auto contents_array = filter_json->getArray("contents");
    if(contents_array->size() < 1)
        throw std::runtime_error("contents_array in kIn is empty");

    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        auto var = contents_array->get(a);
        tmp_in.push_back(GetValue_(var));
    }

    current_filters_->get_in().emplace(std::make_pair(filter_json->get("col"), tmp_in));
}

void IdentifyFilter::NotIn_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kNotIn is empty");

    std::vector<Extras::ValuesProperties> tmp_not_in;
    auto contents_array = filter_json->getArray("contents");
    if(contents_array->size() < 1)
        throw std::runtime_error("contents_array in kNotIn is empty");

    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        auto var = contents_array->get(a);
        tmp_not_in.push_back(GetValue_(var));
    }
    current_filters_->get_not_in().emplace(std::make_pair(filter_json->get("col"), tmp_not_in));
}

void IdentifyFilter::Values_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("contents").isEmpty())
        throw std::runtime_error("contents in kValues is empty");

    auto contents_array = filter_json->getArray("contents");
    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        std::vector<Extras::ValuesProperties> tmp_values;

        auto value_array = contents_array->getArray(a);
        if(value_array->size() < 1)
            throw std::runtime_error("value_array in kValues is empty");

        for(std::size_t b = 0; b < value_array->size(); b++)
        {
            auto var = value_array->get(b);
            tmp_values.push_back(GetValue_(var));
        }

        current_filters_->get_values().push_back(tmp_values);
    }
}

void IdentifyFilter::Set_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kSet is empty");

    auto var = filter_json->get("content");

    current_filters_->get_set().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,GetValue_(var)
    ));
}

void IdentifyFilter::Joins_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("join-type").isEmpty() || filter_json->get("table").isEmpty() || filter_json->get("on").isEmpty())
        throw std::runtime_error("join-type, table or on in kJoins is empty");

    std::map<std::string, Extras::ValuesProperties> tmp_joins;

    auto on_array = filter_json->getArray("on");
    if(on_array->size() < 1)
        throw std::runtime_error("on array in kJoins is empty");

    for(std::size_t a = 0; a < on_array->size(); a++)
    {
        auto on_element = on_array->getObject(a);
        if(on_element->get("col").isEmpty() || on_element->get("value").isEmpty())
            throw std::runtime_error("col or value is empty on array element in kJoins");

        tmp_joins.emplace(std::make_pair
        (
            on_element->get("col").toString()
            ,Extras::ValuesProperties{on_element->get("value").toString(), false}
        ));
    }

    current_filters_->get_joins().push_back
    (
        std::make_pair
        (
            std::array<std::string, 2>{filter_json->get("join-type").toString(), filter_json->get("table").toString()}
            ,tmp_joins
        )
    );
}

void IdentifyFilter::Like_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty())
        throw std::runtime_error("col in kLike is empty");

    auto var = filter_json->get("content");

    current_filters_->get_like().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,GetValue_(var)
    ));
}

void IdentifyFilter::AS_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("content").isEmpty())
        throw std::runtime_error("content in kAS is empty");

    current_filters_->set_as(filter_json->get("content").toString());
}

void IdentifyFilter::Group_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("contents").isEmpty())
        throw std::runtime_error("contents in kGroup is empty");

    auto contents_array = filter_json->getArray("contents");
    if(contents_array->size() < 1)
        throw std::runtime_error("contents_array in kGroup is empty");

    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        current_filters_->get_group_conditions().push_back({contents_array->get(a), false});
    }
}

ValuesProperties IdentifyFilter::GetValue_(Dynamic::Var& var)
{
    Tools::RowValueFormatter row(var);
    row.Format_();

    Extras::ValuesProperties value_properties;
    switch(row.get_row_value_type())
    {
        case Tools::RowValueType::kEmpty:
            return Extras::ValuesProperties{"NULL", false};
            break;
        case Tools::RowValueType::kString:
            return Extras::ValuesProperties{var.toString(), true};
            break;
        case Tools::RowValueType::kInteger:
            return Extras::ValuesProperties{var.toString(), false};
            break;
        case Tools::RowValueType::kFloat:
            return Extras::ValuesProperties{var.toString(), false};
            break;
        default:
            return Extras::ValuesProperties{"", true};
            break;
    }

    return value_properties;
}

void IdentifyFilter::MapFilterTypeFunctors_()
{
    filter_type_functors_.emplace(std::make_pair(FilterType::kFields, [&](Dynamic::Var& filter){Fields_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kPage, [&](Dynamic::Var& filter){Page_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kLimit, [&](Dynamic::Var& filter){Limit_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kSort, [&](Dynamic::Var& filter){Sort_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kIqual, [&](Dynamic::Var& filter){Iqual_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kNotIqual, [&](Dynamic::Var& filter){NotIqual_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kGreatherThan, [&](Dynamic::Var& filter){GreatherThan_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kSmallerThan, [&](Dynamic::Var& filter){SmallerThan_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kBetween, [&](Dynamic::Var& filter){Between_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kIn, [&](Dynamic::Var& filter){In_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kNotIn, [&](Dynamic::Var& filter){NotIn_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kValues, [&](Dynamic::Var& filter){Values_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kSet, [&](Dynamic::Var& filter){Set_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kJoins, [&](Dynamic::Var& filter){Joins_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kLike, [&](Dynamic::Var& filter){Like_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kAS, [&](Dynamic::Var& filter){AS_(filter);}));
    filter_type_functors_.emplace(std::make_pair(FilterType::kGroup, [&](Dynamic::Var& filter){Group_(filter);}));
}
