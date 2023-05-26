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


#include "tools/row_value_formatter.h"

using namespace CPW::Tools;

RowValueFormatter::RowValueFormatter(Poco::Dynamic::Var& value) :
    row_value_type_(RowValueType::kString)
    ,value_string_("")
    ,value_int_(0)
    ,value_float_(0.f)
    ,value_bool_(false)
{
	value_ = &value;
}

RowValueFormatter::~RowValueFormatter()
{

}

void RowValueFormatter::Format_()
{
    std::string val = value_->toString();

    if(value_ == nullptr)
    {
        row_value_type_ = RowValueType::kEmpty;
        return;
    }

    if(value_->isEmpty())
        row_value_type_ = RowValueType::kEmpty;
    else if(value_->isNumeric())
        if(value_->isInteger())
        {
            if(value_->isBoolean())
            {
                value_bool_ = value_->convert<bool>();
                row_value_type_ = RowValueType::kBoolean;
            }
            else
            {
                value_int_ = std::stoi(value_->toString());
                row_value_type_ = RowValueType::kInteger;
            }
        }
        else
        {
            value_float_ = std::stof(value_->toString());
            row_value_type_ = RowValueType::kFloat;
        }
    else if(value_->isDate() || value_->isDateTime())
    {
        DateTime date;
        int diff;

        if(DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, value_->toString(), date, diff))
        {
            DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, value_->toString(), date, diff);
            auto date_string = DateTimeFormatter::format(date, DateTimeFormat::SORTABLE_FORMAT);

            value_string_ = date_string;
            row_value_type_ = RowValueType::kString;
        }
        else
        {
            value_string_ = value_->toString();
            row_value_type_ = RowValueType::kString;
        }
    }
    else
    {
        value_string_ = value_->toString();
        row_value_type_ = RowValueType::kString;
    }
}
