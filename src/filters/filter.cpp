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

#include "filters/filter.h"

using namespace CPW::Filters;
using namespace CPW::Extras;

Filter::Filter() :
    current_filter_type_(FilterType::kUnknown)
{

}

Filter::~Filter()
{

}

bool Filter::FindWHERE_(VectorString& tmp_query)
{
    auto found = std::find(tmp_query.begin(), tmp_query.end(), "WHERE");

    if(found == tmp_query.end())
        return false;

    return true;
}

bool Filter::FindAND_(VectorString& tmp_query)
{
    auto found = std::find(tmp_query.begin(), tmp_query.end(), "WHERE");

    if(*found == tmp_query.back())
        return false;

    return true;
}

CPW::Extras::ValuesProperties Filter::GetValueProperties_(Dynamic::Var& var)
{
    Tools::RowValueFormatter row(var);
    row.Format_();

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
        case Tools::RowValueType::kBoolean:
            return Extras::ValuesProperties{var.toString(), false};
            break;
        default:
            return Extras::ValuesProperties{"", true};
            break;
    }

    return Extras::ValuesProperties{};
}
