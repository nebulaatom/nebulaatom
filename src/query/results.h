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

#ifndef ATOM_QUERY_RESULTS
#define ATOM_QUERY_RESULTS


#include <vector>

#include "query/row.h"


namespace Atom
{
    namespace Query
    {
        class Results;
    }
}


class Atom::Query::Results : public std::vector<Row::Ptr>
{
    public:
        using Ptr = std::shared_ptr<Results>;

        Results();

        Field::Ptr FindField_(Field::Position& field_position);
        Row::Ptr AddRow_();
};

#endif // ATOM_QUERY_RESULTS
