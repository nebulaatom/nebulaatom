/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2023  Jose F Rivas C <josefelixrc7@gmail.com>
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

#ifndef ATOM_QUERY_FIELD
#define ATOM_QUERY_FIELD


#include <string>

#include "tools/dvalue.h"


namespace Atom
{
    namespace Query
    {
        class Field;
    }
}


class Atom::Query::Field
{
    public:
        using Ptr = std::shared_ptr<Field>;

        struct Position
        {
            Position(std::size_t row, std::size_t column) : row(row), column(column) {}

            std::size_t row;
            std::size_t column;
        };

        Field(std::string column_name, Tools::DValue value);

        std::string get_column_name() const { return column_name_;}
        Tools::DValue get_value() const {return value_;}
        Tools::DValue& get_value()
        {
            auto& var = value_;
            return var;
        }

        void set_column_name(std::string column_name) { column_name_ = column_name;}
        void set_value(Tools::DValue value) { value_ = value;}

        std::string String_();
        float Float_();
        bool Bool_();
        int Int_();

    private:
        std::string column_name_;
        Tools::DValue value_;
};


#endif // ATOM_QUERY_FIELD
