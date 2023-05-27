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

#ifndef CPW_FILTERS_VALUES_FILTER_H
#define CPW_FILTERS_VALUES_FILTER_H


#include "filters/filter.h"


namespace CPW
{
    namespace Filters
    {
        class ValuesFilterElement;
        class ValuesFilter;
    }
}


class CPW::Filters::ValuesFilterElement
{
    public:
        enum class Type
        {
            kQuotes
            ,kNoQuotes
        };

        ValuesFilterElement(std::string value, std::string type);


        std::string get_value() const { return value_; }
        Type get_type() const { return type_; }
        std::map<std::string, Type>& get_types()
        {
            auto& var = types_;
            return var;
        }

        void set_value(std::string value) { value_ = value; }
        void set_type(Type type) { type_ = type; }

    protected:
        void AddTypes_();

    private:
        std::string value_;
        Type type_;
        std::map<std::string, Type> types_;
};

class CPW::Filters::ValuesFilter : Filters::Filter
{
    public:
        ValuesFilter();
        virtual ~ValuesFilter();

        std::list<Filters::ValuesFilterElement>& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query) override;

    private:
        std::list<Filters::ValuesFilterElement> filter_elements_;
};


#endif // CPW_FILTERS_VALUES_FILTER_H
