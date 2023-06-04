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


#ifndef CPW_TOOLS_ROWVALUEFORMATTER_H
#define CPW_TOOLS_ROWVALUEFORMATTER_H


#include <string>

#include <Poco/Dynamic/Var.h>
#include "Poco/DateTimeParser.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"


namespace CPW
{
    namespace Tools
    {
        enum class RowValueType;
        class RowValueFormatter;
    }
}

using namespace Poco;


enum class CPW::Tools::RowValueType
{
    kEmpty
    ,kString
    ,kInteger
    ,kFloat
    ,kBoolean
};

class CPW::Tools::RowValueFormatter
{
    public:
        RowValueFormatter();
        RowValueFormatter(Poco::Dynamic::Var& value);
        ~RowValueFormatter();

        RowValueType get_row_value_type() const{return row_value_type_;}
        std::string get_value_string() const{return value_string_;}
        int get_value_int() const{return value_int_;}
        float get_value_float() const{return value_float_;}
        bool get_value_bool() const{return value_bool_;}

        void Format_();

    private:
        RowValueType row_value_type_;
        Poco::Dynamic::Var* value_;

        std::string value_string_;
        int value_int_;
        float value_float_;
        bool value_bool_;

};


#endif // CPW_TOOLS_ROWVALUEFORMATTER_H
