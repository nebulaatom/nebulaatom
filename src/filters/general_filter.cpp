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

#include "filters/general_filter.h"

using namespace CPW::Filters;

GeneralFilter::GeneralFilter()
{
    auto current_filter_type = get_current_filter_type();
    current_filter_type = FilterType::kGeneral;
}

GeneralFilter::~GeneralFilter()
{

}

void GeneralFilter::Identify_(Dynamic::Var& filter)
{
    auto filter_json = get_manage_json().ExtractObject_(filter);

    // Set page
    if(!filter_json->get("page").isEmpty())
        filter_elements_.set_page(filter_json->get("page").toString());

    // Set limit
    if(!filter_json->get("limit").isEmpty())
        filter_elements_.set_limit(filter_json->get("limit").toString());

    // Set as
    if(!filter_json->get("as").isEmpty())
        filter_elements_.set_as(filter_json->get("as").toString());

}

void GeneralFilter::Incorporate_(VectorString& tmp_query)
{
    if(std::stoi(filter_elements_.get_limit()) > 0)
    {
        tmp_query.push_back("LIMIT");
        if(filter_elements_.get_pagination())
        {
            int offset = std::stoi(filter_elements_.get_limit()) * std::stoi(filter_elements_.get_page());
            tmp_query.push_back(std::to_string(offset));
            tmp_query.push_back(", " + filter_elements_.get_limit());
        }
        else
            tmp_query.push_back(filter_elements_.get_limit());
    }
    else
        if(filter_elements_.get_pagination())
            tmp_query.push_back("LIMIT 0, 20");
        else
            tmp_query.push_back("LIMIT 20");
}

void GeneralFilter::IncorporateAS_(VectorString& tmp_query)
{
    if(filter_elements_.get_as() == "")
        return;

    tmp_query.push_back("AS");
    tmp_query.push_back(filter_elements_.get_as());
}
