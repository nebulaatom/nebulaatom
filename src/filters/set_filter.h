/*
* Nebula Atom

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

#ifndef ATOMFILTERS_SET_FILTER
#define ATOMFILTERS_SET_FILTER


#include "filters/filter.h"


namespace Atom
{
    namespace Filters
    {
        class SetFilterElement;
        class SetFilter;
    }
}


class AtomFilters::SetFilterElement
{
    public:
        SetFilterElement(std::string col, Tools::RowValueFormatter);

        std::string get_col() const { return col_; }
        Tools::RowValueFormatter& get_value()
        {
            auto& var = value_;
            return var;
        }

        void set_col(std::string col) { col_ = col; }

    private:
        std::string col_;
        Tools::RowValueFormatter value_;
};

class AtomFilters::SetFilter : Filters::Filter
{
    public:
        SetFilter();
        virtual ~SetFilter();

        std::list<Filters::SetFilterElement>& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters) override;

    private:
        std::list<Filters::SetFilterElement> filter_elements_;
};


#endif // ATOMFILTERS_SET_FILTER
