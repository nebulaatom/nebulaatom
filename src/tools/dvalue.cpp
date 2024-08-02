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


#include "tools/dvalue.h"

using namespace NAF::Tools;

DValue::DValue() :
    type_(Type::kEmpty)
    ,value_string_("")
    ,value_int_(0)
    ,value_float_(0.f)
    ,value_bool_(false)
{
    
}

DValue::DValue(Poco::Dynamic::Var& value) :
    type_(DValue::Type::kString)
    ,value_string_("")
    ,value_int_(0)
    ,value_float_(0.f)
    ,value_bool_(false)
{
    Format_(value);
}

DValue::DValue(std::string value_string) :
    type_(DValue::Type::kString)
    ,value_string_(value_string)
    ,value_int_(0)
    ,value_float_(0.f)
    ,value_bool_(false)
{
    
}

DValue::DValue(const char* value_string) :
    type_(DValue::Type::kString)
    ,value_string_(value_string)
    ,value_int_(0)
    ,value_float_(0.f)
    ,value_bool_(false)
{
    
}

DValue::DValue(int value_int) :
    type_(DValue::Type::kInteger)
    ,value_string_("")
    ,value_int_(value_int)
    ,value_float_(0.f)
    ,value_bool_(false)
{
    
}

DValue::DValue(float value_float) :
    type_(DValue::Type::kFloat)
    ,value_string_("")
    ,value_int_(0)
    ,value_float_(value_float)
    ,value_bool_(false)
{
    
}

DValue::DValue(bool value_bool) :
    type_(DValue::Type::kBoolean)
    ,value_string_("")
    ,value_int_(0)
    ,value_float_(0.f)
    ,value_bool_(value_bool)
{
    
}

bool DValue::operator==(DValue& dvalue)
{
    try
    {
        if(!TypeIsIqual_(dvalue.type_))
        {
            throw std::runtime_error("Error when comparing different types of data.");
            return false;
        }

        switch(dvalue.type_)
        {
            case Type::kEmpty: return true;
            case Type::kBoolean: return value_bool_ == dvalue.get_value_bool();
            case Type::kFloat: return value_float_ == dvalue.get_value_float();
            case Type::kInteger: return value_int_ == dvalue.get_value_int();
            case Type::kString: return value_string_ == dvalue.get_value_string();
        }
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on dvalue.cpp on operator==(): " + std::string(error.what()));
        return false;
    }

    return false;
}

bool DValue::operator!=(DValue& dvalue)
{
    try
    {
        if(!TypeIsIqual_(dvalue.type_))
        {
            throw std::runtime_error("Error when comparing different types of data.");
            return false;
        }

        switch(dvalue.type_)
        {
            case Type::kEmpty: return true;
            case Type::kBoolean: return value_bool_ != dvalue.get_value_bool();
            case Type::kFloat: return value_float_ != dvalue.get_value_float();
            case Type::kInteger: return value_int_ != dvalue.get_value_int();
            case Type::kString: return value_string_ != dvalue.get_value_string();
        }
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on dvalue.cpp on operator==(): " + std::string(error.what()));
        return false;
    }

    return false;
}

bool DValue::operator<(DValue& dvalue)
{
    try
    {
        if(!TypeIsIqual_(dvalue.type_))
        {
            throw std::runtime_error("Error when comparing different types of data.");
            return false;
        }

        switch(dvalue.type_)
        {
            case Type::kEmpty: return true;
            case Type::kBoolean: return value_bool_ < dvalue.get_value_bool();
            case Type::kFloat: return value_float_ < dvalue.get_value_float();
            case Type::kInteger: return value_int_ < dvalue.get_value_int();
            case Type::kString: return value_string_ < dvalue.get_value_string();
        }
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on dvalue.cpp on operator==(): " + std::string(error.what()));
        return false;
    }

    return false;
}

bool DValue::operator<=(DValue& dvalue)
{
    try
    {
        if(!TypeIsIqual_(dvalue.type_))
        {
            throw std::runtime_error("Error when comparing different types of data.");
            return false;
        }

        switch(dvalue.type_)
        {
            case Type::kEmpty: return true;
            case Type::kBoolean: return value_bool_ <= dvalue.get_value_bool();
            case Type::kFloat: return value_float_ <= dvalue.get_value_float();
            case Type::kInteger: return value_int_ <= dvalue.get_value_int();
            case Type::kString: return value_string_ <= dvalue.get_value_string();
        }
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on dvalue.cpp on operator==(): " + std::string(error.what()));
        return false;
    }

    return false;
}

bool DValue::operator>(DValue& dvalue)
{
    try
    {
        if(!TypeIsIqual_(dvalue.type_))
        {
            throw std::runtime_error("Error when comparing different types of data.");
            return false;
        }

        switch(dvalue.type_)
        {
            case Type::kEmpty: return true;
            case Type::kBoolean: return value_bool_ > dvalue.get_value_bool();
            case Type::kFloat: return value_float_ > dvalue.get_value_float();
            case Type::kInteger: return value_int_ > dvalue.get_value_int();
            case Type::kString: return value_string_ > dvalue.get_value_string();
        }
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on dvalue.cpp on operator==(): " + std::string(error.what()));
        return false;
    }

    return false;
}

bool DValue::operator>=(DValue& dvalue)
{
    try
    {
        if(!TypeIsIqual_(dvalue.type_))
        {
            throw std::runtime_error("Error when comparing different types of data.");
            return false;
        }

        switch(dvalue.type_)
        {
            case Type::kEmpty: return true;
            case Type::kBoolean: return value_bool_ >= dvalue.get_value_bool();
            case Type::kFloat: return value_float_ >= dvalue.get_value_float();
            case Type::kInteger: return value_int_ >= dvalue.get_value_int();
            case Type::kString: return value_string_ >= dvalue.get_value_string();
        }
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on dvalue.cpp on operator==(): " + std::string(error.what()));
        return false;
    }

    return false;
}

bool DValue::TypeIsIqual_(DValue::Type row_value_type)
{
    return row_value_type == type_;
}

std::string DValue::ToString_()
{
    std::string final_string = "";
    switch(type_)
    {
        case Type::kEmpty:
        {
            try
            {
                throw std::runtime_error("Erro to convert Empty value to String.");
            }
            catch(std::exception& error)
            {
                Tools::OutputLogger::Error_("Error on dvalue.cpp on ToString_(): " + std::string(error.what()));
            }
            break;
        }
        case Type::kBoolean:
        {
            try
            {
                final_string = std::to_string(value_bool_);
            }
            catch(std::exception& error)
            {
                Tools::OutputLogger::Error_("Error on dvalue.cpp on ToString_(): " + std::string(error.what()));
            }
            break;
        }
        case Type::kFloat:
        {
            try
            {
                final_string = std::to_string(value_float_);
            }
            catch(std::exception& error)
            {
                Tools::OutputLogger::Error_("Error on dvalue.cpp on ToString_(): " + std::string(error.what()));
            }
            break;
        }
        case Type::kInteger:
        {
            try
            {
                final_string = std::to_string(value_int_);
            }
            catch(std::exception& error)
            {
                Tools::OutputLogger::Error_("Error on dvalue.cpp on ToString_(): " + std::string(error.what()));
            }
            break;
        }
        case Type::kString:
        {
            try
            {
                final_string = value_string_;
            }
            catch(std::exception& error)
            {
                Tools::OutputLogger::Error_("Error on dvalue.cpp on ToString_(): " + std::string(error.what()));
            }
            break;
        }
    }

    return final_string;
}

void DValue::Format_(Poco::Dynamic::Var& value)
{
    if(value.isEmpty())
        type_ = DValue::Type::kEmpty;
    else if(value.isNumeric())
        if(value.isInteger())
        {
            if(value.isBoolean())
            {
                value_bool_ = value.convert<bool>();
                type_ = DValue::Type::kBoolean;
            }
            else
            {
                value_int_ = std::stoi(value.toString());
                type_ = DValue::Type::kInteger;
            }
        }
        else
        {
            value_float_ = std::stof(value.toString());
            type_ = DValue::Type::kFloat;
        }
    else if(value.isDate() || value.isDateTime())
    {
        DateTime date;
        int diff;

        if(DateTimeParser::tryParse(DateTimeFormat::ISO8601_FORMAT, value.toString(), date, diff))
        {
            DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, value.toString(), date, diff);
            auto date_string = DateTimeFormatter::format(date, DateTimeFormat::SORTABLE_FORMAT);

            value_string_ = date_string;
            type_ = DValue::Type::kString;
        }
        else
        {
            value_string_ = value.toString();
            type_ = DValue::Type::kString;
        }
    }
    else
    {
        value_string_ = value.toString();
        type_ = DValue::Type::kString;
    }
}
