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

#ifndef CPW_FILTERS_INCORPORATEFILTERS_H
#define CPW_FILTERS_INCORPORATEFILTERS_H


#include <string>
#include <vector>
#include <memory>

#include "filters/filters.h"


namespace CPW
{
    namespace Filters
    {
        class IncorporateFilters;
    }
}


class CPW::Filters::IncorporateFilters
{
    public:
        using FiltersSharedPtr = std::shared_ptr<Filters>;
        using VectorString = std::vector<std::string>;

        IncorporateFilters(FiltersSharedPtr filters);
        ~IncorporateFilters();

        void IncorporateWhere_(VectorString& tmp_query);
        void IncorporateAND_(VectorString& tmp_query);
        void IncorporateFields_(VectorString& tmp_query);
        void IncorporatePageLimit_(VectorString& tmp_query, bool pagination);
        void IncorporateSort_(VectorString& tmp_query);
        void IncorporateIqual_(VectorString& tmp_query);
        void IncorporateNotIqual_(VectorString& tmp_query);
        void IncorporateGreatherThan_(VectorString& tmp_query);
        void IncorporateSmallerThan_(VectorString& tmp_query);
        void IncorporateBetween_(VectorString& tmp_query);
        void IncorporateIn_(VectorString& tmp_query);
        void IncorporateNotIn_(VectorString& tmp_query);
        void IncorporateValues_(VectorString& tmp_query);
        void IncorporateSet_(VectorString& tmp_query);
        void IncorporateJoins_(VectorString& tmp_query);

    private:
        FiltersSharedPtr current_filters_;
};

#endif // CPW_FILTERS_INCORPORATEFILTERS_H
