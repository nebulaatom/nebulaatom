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

#ifndef CPW_FILTERS_GROUP_FILTER_H
#define CPW_FILTERS_GROUP_FILTER_H


#include "filters/filter.h"


namespace CPW
{
    namespace Filters
    {
        class GroupFilterElement;
        class GroupFilter;
    }
}


class CPW::Filters::GroupFilterElement
{
    public:
        enum class Type
        {
            kAsc
            ,kDesc
        };

        GroupFilterElement(std::string col, std::string type);


        std::string get_col() const { return col_; }
        Type get_type() const { return type_; }
        std::map<std::string, Type>& get_types()
        {
            auto& var = types_;
            return var;
        }

        void set_col(std::string col) { col_ = col; }
        void set_type(Type type) { type_ = type; }

    protected:
        void AddTypes_();

    private:
        std::string col_;
        Type type_;
        std::map<std::string, Type> types_;
};

class CPW::Filters::GroupFilter : Filters::Filter
{
    public:
        GroupFilter();
        virtual ~GroupFilter();

        std::list<Filters::GroupFilterElement>& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query) override;

    private:
        std::list<Filters::GroupFilterElement> filter_elements_;
};


#endif // CPW_FILTERS_GROUP_FILTER_H
