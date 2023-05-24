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
        class GeneralFilter;
    }
}


class CPW::Filters::GeneralFilter : Filters::Filter
{
    public:
        GeneralFilter();
        virtual ~GeneralFilter();

        std::string get_page(){ return page_; }
        std::string get_limit() { return limit_; }
        std::string get_as() { return as_; }
        bool get_pagination() { return pagination_; }

        void set_page(std::string page){ page_ = page; }
        void set_limit(std::string limit) { limit_ = limit; }
        void set_as(std::string as) { as_ = as; }
        void set_pagination(bool pagination) { pagination_ = pagination; }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query) override;
        void IncorporateAS_(VectorString& tmp_query);

    private:
        std::string page_;
        std::string limit_;
        std::string as_;
        bool pagination_;
};


#endif // CPW_FILTERS_GENERAL_FILTER_H
