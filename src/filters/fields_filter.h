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

#ifndef ATOMFILTERS_FIELDS_FILTER
#define ATOMFILTERS_FIELDS_FILTER


#include "filters/filter.h"


namespace Atom
{
    namespace Filters
    {
        class FieldsFilterElement;
        class FieldsFilter;
    }
}


class AtomFilters::FieldsFilterElement
{
    public:
        enum class Type
        {
            kQuotes
            ,kNoQuotes
        };

            FieldsFilterElement(std::string field);
            FieldsFilterElement(std::string field, std::string as, std::string type);

        std::string& get_field()
        {
            auto& var = field_;
            return var;
        }
        std::string get_as() const { return as_; }
        Type get_type() const { return type_; }
        std::map<std::string, Type>& get_types()
        {
            auto& var = types_;
            return var;
        }

        void set_as(std::string as) { as_ = as; }
        void set_type(Type type) { type_ = type; }

    protected:
        void AddTypes_();

    private:
        std::string field_;
        std::string as_;
        Type type_;
        std::map<std::string, Type> types_;
};

class AtomFilters::FieldsFilter : Filters::Filter
{
    public:
        FieldsFilter();
        virtual ~FieldsFilter();

        std::list<Filters::FieldsFilterElement>& get_filter_elements()
        {
            auto& var = filter_elements_;
            return var;
        }

        virtual void Identify_(Dynamic::Var& filter) override;
        virtual void Incorporate_(VectorString& tmp_query, RowValueFormatterList& query_parameters) override;

    private:
        std::list<Filters::FieldsFilterElement> filter_elements_;
};


#endif // ATOMFILTERS_FIELDS_FILTER
