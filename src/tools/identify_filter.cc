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

#include "tools/identify_filter.h"

using namespace CPW::Tools;

IdentifyFilter::IdentifyFilter(std::shared_ptr<Tools::Filters> current_filters) :
    current_filters_(current_filters)
{
    MapFilterTypeFunctors_();
}

IdentifyFilter::~IdentifyFilter()
{

}

void IdentifyFilter::Fields_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("contents").isEmpty())
        throw std::runtime_error("contents in kFields is empty");

    auto contents_array = filter_json->getArray("contents");
    if(contents_array->size() < 1)
        throw std::runtime_error("contents_array in kFields is empty");

    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        current_filters_->get_fields().push_back({contents_array->get(a), false});
    }
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
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("contents").isEmpty())
        throw std::runtime_error("contents in kSort is empty");

    auto contents_array = filter_json->getArray("contents");
    if(contents_array->size() < 1)
        throw std::runtime_error("contents_array in kSort is empty");

    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        current_filters_->get_sorts_conditions().push_back({contents_array->get(a), false});
    }
}

void IdentifyFilter::Iqual_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("content").isEmpty() || filter_json->get("col").isEmpty())
        throw std::runtime_error("content or col in kIqual is empty");

    current_filters_->get_iquals_conditions().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,Extras::ValuesProperties{filter_json->get("content").toString(), true}
    ));
}

void IdentifyFilter::NotIqual_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("content").isEmpty() || filter_json->get("col").isEmpty())
        throw std::runtime_error("content or col in kNotIqual is empty");

    current_filters_->get_not_iquals_conditions().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,Extras::ValuesProperties{filter_json->get("content").toString(), true}
    ));
}

void IdentifyFilter::GreatherThan_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("content").isEmpty() || filter_json->get("col").isEmpty())
        throw std::runtime_error("content or col in kGreatherThan is empty");

    current_filters_->get_greather_than().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,Extras::ValuesProperties{filter_json->get("content").toString(), true}
    ));
}

void IdentifyFilter::SmallerThan_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("content").isEmpty() || filter_json->get("col").isEmpty())
        throw std::runtime_error("content or col in kSmallerThan is empty");

    current_filters_->get_smaller_than().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,Extras::ValuesProperties{filter_json->get("content").toString(), true}
    ));
}

void IdentifyFilter::Between_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("col").isEmpty() || filter_json->get("content1").isEmpty() || filter_json->get("content2").isEmpty())
        throw std::runtime_error("col, content1 or content2 in kBetween is empty");

    current_filters_->get_between().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,std::make_pair
        (
            Extras::ValuesProperties{filter_json->get("content1").toString(), true}
            ,Extras::ValuesProperties{filter_json->get("content2").toString(), true}
        )
    ));
}

void IdentifyFilter::In_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("contents").isEmpty() || filter_json->get("col").isEmpty())
        throw std::runtime_error("col or contents in kIn is empty");

    std::vector<Extras::ValuesProperties> tmp_in;
    auto contents_array = filter_json->getArray("contents");
    if(contents_array->size() < 1)
        throw std::runtime_error("contents_array in kIn is empty");

    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        tmp_in.push_back({contents_array->get(a), true});
    }

    current_filters_->get_in().emplace(std::make_pair(filter_json->get("col"), tmp_in));
}

void IdentifyFilter::NotIn_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("contents").isEmpty() || filter_json->get("col").isEmpty())
        throw std::runtime_error("col or contents in kNotIn is empty");

    std::vector<Extras::ValuesProperties> tmp_not_in;
    auto contents_array = filter_json->getArray("contents");
    if(contents_array->size() < 1)
        throw std::runtime_error("contents_array in kNotIn is empty");

    for(std::size_t a = 0; a < contents_array->size(); a++)
    {
        tmp_not_in.push_back({contents_array->get(a), true});
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
        for(std::size_t b = 0; b < value_array->size(); b++)
        {
            tmp_values.push_back({value_array->get(b).toString(), true});
        }

        current_filters_->get_values().push_back(tmp_values);
    }
}

void IdentifyFilter::Set_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("content").isEmpty() || filter_json->get("col").isEmpty())
        throw std::runtime_error("content or col in kSet is empty");

    current_filters_->get_set().emplace(std::make_pair
    (
        filter_json->get("col").toString()
        ,Extras::ValuesProperties{filter_json->get("content").toString(), true}
    ));
}

void IdentifyFilter::Joins_(Dynamic::Var& filter)
{
    auto filter_json = manage_json_.ExtractObject_(filter);
    if(filter_json->get("join-type").isEmpty() || filter_json->get("table").isEmpty() || filter_json->get("on").isEmpty())
        throw std::runtime_error("join-type, table or on in kJoins is empty");

    std::map<std::string, Extras::ValuesProperties> tmp_joins;

    auto on_array = manage_json_.ExtractArray_(filter_json->get("on"));
    for(std::size_t a = 0; a < on_array->size(); a++)
    {
        auto on_element = on_array->getObject(a);

        tmp_joins.emplace(std::make_pair
        (
            on_element->get("col").toString()
            ,Extras::ValuesProperties{on_element->get("value").toString(), false}
        ));
    }

    current_filters_->get_joins().emplace
    (
        std::make_pair
        (
            std::array<std::string, 2>{filter_json->get("join-type").toString(), filter_json->get("table").toString()}
            ,tmp_joins
        )
    );
}

void IdentifyFilter::MapFilterTypeFunctors_()
{
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kFields, [&](Dynamic::Var& filter){Fields_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kPage, [&](Dynamic::Var& filter){Page_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kLimit, [&](Dynamic::Var& filter){Limit_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kSort, [&](Dynamic::Var& filter){Sort_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kIqual, [&](Dynamic::Var& filter){Iqual_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kNotIqual, [&](Dynamic::Var& filter){NotIqual_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kGreatherThan, [&](Dynamic::Var& filter){GreatherThan_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kSmallerThan, [&](Dynamic::Var& filter){SmallerThan_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kBetween, [&](Dynamic::Var& filter){Between_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kIn, [&](Dynamic::Var& filter){In_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kNotIn, [&](Dynamic::Var& filter){NotIn_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kValues, [&](Dynamic::Var& filter){Values_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kSet, [&](Dynamic::Var& filter){Set_(filter);}));
    filter_type_functors_.emplace(std::make_pair(Tools::FilterType::kJoins, [&](Dynamic::Var& filter){Joins_(filter);}));
}
