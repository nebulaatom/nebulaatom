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

void FieldsFilter::Add_(std::string value, std::string as)
{
    Filters::Field field;
    field.value_ = Extras::ValuesProperties{value, false};
    field.as_ = as;
    fields_.push_back(std::move(field));
}
