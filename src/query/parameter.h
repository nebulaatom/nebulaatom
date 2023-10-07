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

#ifndef CPW_QUERY_PARAMETER_H
#define CPW_QUERY_PARAMETER_H


#include <string>

#include "tools/row_value_formatter.h"


namespace CPW
{
    namespace Query
    {
        class Parameter;
    }
}


class CPW::Query::Parameter
{
    public:
        Parameter(std::string name, Tools::RowValueFormatter value);

        Tools::RowValueFormatter& get_value()
        {
            auto& var = value_;
            return var;
        }

    private:
        std::string name_;
        Tools::RowValueFormatter value_;
};

#endif // CPW_QUERY_PARAMETER_H
