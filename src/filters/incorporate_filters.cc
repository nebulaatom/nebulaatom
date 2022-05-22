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

#include "filters/incorporate_filters.h"

using namespace CPW::Filters;

IncorporateFilters::IncorporateFilters(FiltersSharedPtr filters) :
    current_filters_(filters)
{

}

IncorporateFilters::~IncorporateFilters()
{

}

void IncorporateFilters::IncorporateWhere_(VectorString& tmp_query)
{
    auto found = std::find(tmp_query.begin(), tmp_query.end(), "WHERE");

    if(found == tmp_query.end())
        tmp_query.push_back("WHERE");
}

void IncorporateFilters::IncorporateAND_(VectorString& tmp_query)
{
    auto found = std::find(tmp_query.begin(), tmp_query.end(), "WHERE");

    if(*found != tmp_query.back())
        tmp_query.push_back("AND");
}

void IncorporateFilters::IncorporateFields_(VectorString& tmp_query)
{
    if(current_filters_->get_fields().size() == 0)
        tmp_query.push_back("*");
    else
    {
        for(auto it : current_filters_->get_fields())
        {
            if(it != *current_filters_->get_fields().begin())
                tmp_query.push_back(",");

            tmp_query.push_back(it.GetFinalValue());
        }
    }
}

void IncorporateFilters::IncorporatePageLimit_(VectorString& tmp_query, bool pagination)
{
    if(std::stoi(current_filters_->get_limit()) > 0)
    {
        tmp_query.push_back("LIMIT");
        if(pagination)
        {
            int offset = std::stoi(current_filters_->get_limit()) * std::stoi(current_filters_->get_page());
            tmp_query.push_back(std::to_string(offset));
            tmp_query.push_back(", " + current_filters_->get_limit());
        }
        else
            tmp_query.push_back(current_filters_->get_limit());
    }
    else
        if(pagination)
            tmp_query.push_back("LIMIT 0, 20");
        else
            tmp_query.push_back("LIMIT 20");
}

void IncorporateFilters::IncorporateSort_(VectorString& tmp_query)
{
    if(current_filters_->get_sorts_conditions().size() > 0)
    {
        tmp_query.push_back("ORDER BY");
        for(auto it : current_filters_->get_sorts_conditions())
        {
            if(it != *current_filters_->get_sorts_conditions().begin())
                tmp_query.push_back(",");

            tmp_query.push_back(it.GetFinalValue());
        }
    }
}

void IncorporateFilters::IncorporateIqual_(VectorString& tmp_query)
{
    if(current_filters_->get_iquals_conditions().size() > 0)
    {
        IncorporateWhere_(tmp_query);

        for(auto it : current_filters_->get_iquals_conditions())
        {
            IncorporateAND_(tmp_query);

            tmp_query.push_back(it.first);
            tmp_query.push_back("=");
            tmp_query.push_back(it.second.GetFinalValue());
        }
    }
}

void IncorporateFilters::IncorporateNotIqual_(VectorString& tmp_query)
{
    if(current_filters_->get_not_iquals_conditions().size() > 0)
    {
        IncorporateWhere_(tmp_query);

        for(auto it : current_filters_->get_not_iquals_conditions())
        {
            IncorporateAND_(tmp_query);

            tmp_query.push_back(it.first);
            tmp_query.push_back("<>");
            tmp_query.push_back(it.second.GetFinalValue());
        }
    }
}

void IncorporateFilters::IncorporateGreatherThan_(VectorString& tmp_query)
{
    if(current_filters_->get_greather_than().size() > 0)
    {
        IncorporateWhere_(tmp_query);

        for(auto it : current_filters_->get_greather_than())
        {
            IncorporateAND_(tmp_query);

            tmp_query.push_back(it.first);
            tmp_query.push_back(">");
            tmp_query.push_back(it.second.GetFinalValue());
        }
    }
}

void IncorporateFilters::IncorporateSmallerThan_(VectorString& tmp_query)
{
    if(current_filters_->get_smaller_than().size() > 0)
    {
        IncorporateWhere_(tmp_query);

        for(auto it : current_filters_->get_smaller_than())
        {
            IncorporateAND_(tmp_query);

            tmp_query.push_back(it.first);
            tmp_query.push_back("<");
            tmp_query.push_back(it.second.GetFinalValue());
        }
    }
}

void IncorporateFilters::IncorporateBetween_(VectorString& tmp_query)
{
    if(current_filters_->get_between().size() > 0)
    {
        IncorporateWhere_(tmp_query);

        for(auto it : current_filters_->get_between())
        {
            IncorporateAND_(tmp_query);

            tmp_query.push_back(it.first);
            tmp_query.push_back("BETWEEN");
            tmp_query.push_back(it.second.first.GetFinalValue());
            tmp_query.push_back("AND");
            tmp_query.push_back(it.second.second.GetFinalValue());
        }
    }
}

void IncorporateFilters::IncorporateIn_(VectorString& tmp_query)
{
    if(current_filters_->get_in().size() > 0)
    {
        IncorporateWhere_(tmp_query);

        for(auto it : current_filters_->get_in())
        {
            IncorporateAND_(tmp_query);

            tmp_query.push_back(it.first);
            tmp_query.push_back("IN (");
            for(auto it_v : it.second)
            {
                if(it_v != *it.second.begin())
                    tmp_query.push_back(",");

                tmp_query.push_back(it_v.GetFinalValue());
            }
            tmp_query.push_back(")");
        }
    }
}

void IncorporateFilters::IncorporateNotIn_(VectorString& tmp_query)
{
    if(current_filters_->get_not_in().size() > 0)
    {
        IncorporateWhere_(tmp_query);

        for(auto it : current_filters_->get_not_in())
        {
            IncorporateAND_(tmp_query);

            tmp_query.push_back(it.first);
            tmp_query.push_back("NOT IN (");
            for(auto it_v : it.second)
            {
                if(it_v != *it.second.begin())
                    tmp_query.push_back(",");

                tmp_query.push_back(it_v.GetFinalValue());
            }
            tmp_query.push_back(")");
        }
    }
}

void IncorporateFilters::IncorporateValues_(VectorString& tmp_query)
{
    if(current_filters_->get_values().size() > 0)
    {
        for(auto it = current_filters_->get_values().begin(); it != current_filters_->get_values().end(); ++it)
        {
            if(&*it != &*current_filters_->get_values().begin())
                tmp_query.push_back(",");

            tmp_query.push_back("(");
            for(auto it_sub = it->begin(); it_sub != it->end(); ++it_sub)
            {
                if(it_sub != it->begin())
                    tmp_query.push_back(",");

                tmp_query.push_back(it_sub->GetFinalValue());
            }
            tmp_query.push_back(")");
        }
    }
}

void IncorporateFilters::IncorporateSet_(VectorString& tmp_query)
{
    if(current_filters_->get_set().size() > 0)
    {
        for(auto& it : current_filters_->get_set())
        {
            if(&it != &*current_filters_->get_set().begin())
                tmp_query.push_back(",");

            tmp_query.push_back(it.first);
            tmp_query.push_back("=");
            tmp_query.push_back(it.second.GetFinalValue());
        }
    }
}

void IncorporateFilters::IncorporateJoins_(VectorString& tmp_query)
{
    if(current_filters_->get_joins().size() > 0)
    {
        for(auto it : current_filters_->get_joins())
        {
            tmp_query.push_back(it.first[0]);
            tmp_query.push_back("JOIN");
            tmp_query.push_back(it.first[1]);
            tmp_query.push_back("ON (");
            for(auto it_v : it.second)
            {
                if(it_v.first != it.second.begin()->first)
                    tmp_query.push_back("AND");

                tmp_query.push_back(it_v.first);
                tmp_query.push_back("=");
                tmp_query.push_back(it_v.second.GetFinalValue());
            }
            tmp_query.push_back(")");
        }
    }
}

void IncorporateFilters::IncorporateLike_(VectorString& tmp_query)
{
    if(current_filters_->get_like().size() > 0)
    {
        IncorporateWhere_(tmp_query);

        for(auto it : current_filters_->get_like())
        {
            IncorporateAND_(tmp_query);

            tmp_query.push_back(it.first);
            tmp_query.push_back("LIKE");
            tmp_query.push_back(it.second.GetFinalValue());
        }
    }
}
