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

#ifndef CPW_FILTERS_GENERAL_FILTER_H
#define CPW_FILTERS_GENERAL_FILTER_H


#include "filters/filter.h"


namespace CPW
{
    namespace Filters
    {
        class GeneralFilterElements;
        class GeneralFilter;
    }
}


class CPW::Filters::GeneralFilterElements
{
    public:
        std::string page_;
        std::string limit_;
        std::string as_;
        bool pagination_;
};

class CPW::Filters::GeneralFilter : Filters::Filter
{
    public:
        GeneralFilter();
        virtual ~GeneralFilter();

        Filters::GeneralFilterElements& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query) override;
        void IncorporateAS_(VectorString& tmp_query);

    private:
        Filters::GeneralFilterElements filter_elements_;
};


#endif // CPW_FILTERS_GENERAL_FILTER_H