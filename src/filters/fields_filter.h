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

#ifndef CPW_FILTERS_FIELDS_FILTER_H
#define CPW_FILTERS_FIELDS_FILTER_H


#include "filters/filter.h"


namespace CPW
{
    namespace Filters
    {
        class FieldsFilterElements;
        class FieldsFilter;
    }
}


class CPW::Filters::FieldsFilterElements
{
    public:
        struct Field
        {
            Field(Extras::ValuesProperties value, std::string as) : value(value), as(as){}
            Field() : value("", true), as(""){}

            Extras::ValuesProperties value;
            std::string as;
        };

        std::list<Field> fields_;
        bool all_;
};

class CPW::Filters::FieldsFilter : Filters::Filter
{
    public:
        FieldsFilter();
        virtual ~FieldsFilter();

        Filters::FieldsFilterElements& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query) override;

    protected:
        void Add_(std::string value, std::string as);

    private:
        Filters::FieldsFilterElements filter_elements_;
};


#endif // CPW_FILTERS_FIELDS_FILTER_H
