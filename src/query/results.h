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
        class Row;
        class Results;
    }
}


class CPW::Query::Row
{
    public:
        Row();

        std::vector<Tools::RowValueFormatter>& get_fields()
        {
            auto& var = fields_;
            return var;
        }

    private:
        std::vector<Tools::RowValueFormatter> fields_;
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

    private:
        std::vector<Row> rows_;
};

#endif // CPW_QUERY_RESULTS_H
