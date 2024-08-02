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

#ifndef NAF_QUERY_ROW
#define NAF_QUERY_ROW


#include <vector>

#include "query/field.h"
#include "tools/dvalue.h"


namespace NAF
{
    namespace Query
    {
        class Row;
    }
}


class NAF::Query::Row : public std::vector<Field::Ptr>
{
    public:
        using Ptr = std::shared_ptr<Row>;

        Row();

        Field::Ptr FindField_(std::string column_name);
        Field::Ptr AddField_(std::string column_name, Tools::DValue value);
};

#endif // NAF_QUERY_ROW
