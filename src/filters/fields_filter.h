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
        class Field;
        class FieldsFilter;
    }
}



class CPW::Filters::Field
{
    public:
        Field(Extras::ValuesProperties value, std::string as) :
            value_(value)
            ,as_(as)
        {

        }
        Field() :
            value_("", true)
            ,as_("")
        {

        }

        Extras::ValuesProperties value_;
        std::string as_;
};

class CPW::Filters::FieldsFilter : Filters::Filter
{
    public:
        FieldsFilter();
        virtual ~FieldsFilter();

        std::list<Field>& get_fields()
        {
            auto& var = fields_;
            return var;
        }

        void Add_(std::string value, std::string as);

    private:
        std::list<Field> fields_;
};


#endif // CPW_FILTERS_FIELDS_FILTER_H
