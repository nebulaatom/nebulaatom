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


#ifndef NAF_TOOLS_DVALUE
#define NAF_TOOLS_DVALUE


#include <string>
#include <stdexcept>

#include <Poco/Dynamic/Var.h>
#include "Poco/DateTimeParser.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"

#include "tools/output_logger.h"


namespace NAF
{
    namespace Tools
    {
        class DValue;
    }
}

using namespace Poco;


class NAF::Tools::DValue
{
    public:
        enum class Type
        {
            kEmpty
            ,kString
            ,kInteger
            ,kFloat
            ,kBoolean
        };

        DValue();
        DValue(Poco::Dynamic::Var& value);
        DValue(std::string value_string);
        DValue(char value_string);
        DValue(const char* value_string);
        DValue(int value_int);
        DValue(float value_float);
        DValue(bool value_bool);
        ~DValue(){}

        Type& get_type()
        {
            auto& var = type_;
            return var;
        }
        std::string& get_value_string()
        {
            auto& var = value_string_;
            return var;
        }
        int& get_value_int()
        {
            auto& var = value_int_;
            return var;
        }
        float& get_value_float()
        {
            auto& var = value_float_;
            return var;
        }
        bool& get_value_bool()
        {
            auto& var = value_bool_;
            return var;
        }

        bool operator==(DValue& dvalue);
        bool operator!=(DValue& dvalue);
        bool operator<(DValue& dvalue);
        bool operator<=(DValue& dvalue);
        bool operator>(DValue& dvalue);
        bool operator>=(DValue& dvalue);
        bool TypeIsIqual_(Type row_value_type);
        std::string ToString_();

    protected:
        void Format_(Poco::Dynamic::Var& value);

    private:
        Type type_;

        std::string value_string_;
        int value_int_;
        float value_float_;
        bool value_bool_;

};


#endif // NAF_TOOLS_DVALUE
