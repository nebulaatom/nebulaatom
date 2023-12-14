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

#ifndef CPW_QUERY_RESULTS_H
#define CPW_QUERY_RESULTS_H


#include <vector>

#include "tools/row_value_formatter.h"


namespace CPW
{
    namespace Query
    {
        class ConditionalField;
        class Field;
        class Row;
        class Results;
    }
}


class CPW::Query::ConditionalField
{
    public:
        ConditionalField(std::size_t row, std::size_t column) : row_(row), column_(column) {}

        std::size_t get_row() const { return row_; }
        std::size_t get_column() const { return column_; }

        void set_row(std::size_t row) { row_ = row; }
        void set_column(std::size_t column) { column_ = column; }

    private:
        std::size_t row_;
        std::size_t column_;
};

class CPW::Query::Field
{
    public:
        Field(std::string column_name, Tools::RowValueFormatter value);

        std::string get_column_name() const { return column_name_;}
        Tools::RowValueFormatter get_value() const {return value_;}
        Tools::RowValueFormatter& get_value()
        {
            auto& var = value_;
            return var;
        }

        void set_column_name(std::string column_name) { column_name_ = column_name;}
        void set_value(Tools::RowValueFormatter value) { value_ = value;}

    private:
        std::string column_name_;
        Tools::RowValueFormatter value_;
};

class CPW::Query::Row
{
    public:
        Row();

        std::vector<Field>& get_fields()
        {
            auto& var = fields_;
            return var;
        }

        Field FindField_(std::string column_name);

    private:
        std::vector<Field> fields_;
};

class CPW::Query::Results
{
    public:
        Results();

        std::vector<Row>& get_rows()
        {
            auto& var = rows_;
            return var;
        }

        std::shared_ptr<Field> FindField_(ConditionalField& field);

    private:
        std::vector<Row> rows_;
};

#endif // CPW_QUERY_RESULTS_H
