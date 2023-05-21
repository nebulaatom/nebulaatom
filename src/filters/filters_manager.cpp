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

#include "filters/filters_manager.h"

using namespace CPW::Filters;

FiltersManager::FiltersManager() :
    page_("0")
    ,limit_("20")
    ,as_("")
{
    fields_ = std::make_shared<FieldsFilter>();
    MapFilterType_();
}

FiltersManager::~FiltersManager()
{

}

void FiltersManager::MapFilterType_()
{
    filters_type_map_.emplace(std::make_pair("fields", FilterType::kFields));
    filters_type_map_.emplace(std::make_pair("page", FilterType::kPage));
    filters_type_map_.emplace(std::make_pair("limit", FilterType::kLimit));
    filters_type_map_.emplace(std::make_pair("sort", FilterType::kSort));
    filters_type_map_.emplace(std::make_pair("iqual", FilterType::kIqual));
    filters_type_map_.emplace(std::make_pair("notiqual", FilterType::kNotIqual));
    filters_type_map_.emplace(std::make_pair("greatherthan", FilterType::kGreatherThan));
    filters_type_map_.emplace(std::make_pair("smallerthan", FilterType::kSmallerThan));
    filters_type_map_.emplace(std::make_pair("between", FilterType::kBetween));
    filters_type_map_.emplace(std::make_pair("in", FilterType::kIn));
    filters_type_map_.emplace(std::make_pair("notin", FilterType::kNotIn));
    filters_type_map_.emplace(std::make_pair("values", FilterType::kValues));
    filters_type_map_.emplace(std::make_pair("set", FilterType::kSet));
    filters_type_map_.emplace(std::make_pair("joins", FilterType::kJoins));
    filters_type_map_.emplace(std::make_pair("like", FilterType::kLike));
    filters_type_map_.emplace(std::make_pair("as", FilterType::kAS));
    filters_type_map_.emplace(std::make_pair("group", FilterType::kGroup));
}

bool FiltersManager::ExistsType_(std::string type)
{
    auto found = filters_type_map_.find(type);
    if(found != filters_type_map_.end())
        return true;
    else
        return false;
}
