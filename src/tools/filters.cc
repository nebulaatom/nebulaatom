/*
* <one line to give the program's name and a brief idea of what it does.>
* Copyright (C) 2021  <copyright holder> <email>
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

#include "tools/filters.h"

using namespace CPW::Tools;

Filters::Filters() :
    page_("0")
    ,limit_("20")
{
    MapFilterType_();
}

Filters::~Filters()
{

}

void Filters::MapFilterType_()
{
    filters_type_map_.emplace(std::make_pair("fields", Tools::FilterType::kFields));
    filters_type_map_.emplace(std::make_pair("page", Tools::FilterType::kPage));
    filters_type_map_.emplace(std::make_pair("limit", Tools::FilterType::kLimit));
    filters_type_map_.emplace(std::make_pair("sort", Tools::FilterType::kSort));
    filters_type_map_.emplace(std::make_pair("iqual", Tools::FilterType::kIqual));
    filters_type_map_.emplace(std::make_pair("notiqual", Tools::FilterType::kNotIqual));
    filters_type_map_.emplace(std::make_pair("greatherthan", Tools::FilterType::kGreatherThan));
    filters_type_map_.emplace(std::make_pair("smallerthan", Tools::FilterType::kSmallerThan));
    filters_type_map_.emplace(std::make_pair("between", Tools::FilterType::kBetween));
    filters_type_map_.emplace(std::make_pair("in", Tools::FilterType::kIn));
    filters_type_map_.emplace(std::make_pair("notin", Tools::FilterType::kNotIn));
    filters_type_map_.emplace(std::make_pair("values", Tools::FilterType::kValues));
    filters_type_map_.emplace(std::make_pair("set", Tools::FilterType::kSet));
    filters_type_map_.emplace(std::make_pair("joins", Tools::FilterType::kJoins));
}

bool Filters::ExistsType_(std::string type)
{
    auto found = filters_type_map_.find(type);
    if(found != filters_type_map_.end())
        return true;
    else
        return false;
}
