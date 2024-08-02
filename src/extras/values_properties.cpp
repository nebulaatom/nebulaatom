/*
* <one line to give the program's name and a brief idea of what it does.>
* Copyright (C) 2021  <copyright holder> <email>
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

#include "extras/values_properties.h"

using namespace NAF::Extras;

ValuesProperties::ValuesProperties() :
    value_("")
    ,quotes_(true)
{

}

ValuesProperties::ValuesProperties(std::string value, bool quotes) :
    value_(value)
    ,quotes_(quotes)
{

}

ValuesProperties::~ValuesProperties()
{

}

std::string ValuesProperties::GetFinalValue()
{
    if(quotes_)
        return "'" + value_ + "'";
    else
        return value_;
}

std::string ValuesProperties::GetFinalValue() const
{
    if(quotes_)
        return "'" + value_ + "'";
    else
        return value_;
}
