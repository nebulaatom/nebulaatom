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

#include "http/methods.h"
#include <exception>
#include <stdexcept>

using namespace NAF::HTTP;

Methods::Methods()
{
    MapMethods_();
}

EnumMethods Methods::GetMethod_(const std::string method)
{
    auto found = map_string_methods_.find(method);
    if(found != map_string_methods_.end())
        return found->second;
    else
    {
        return EnumMethods::kNULL;
        throw std::runtime_error("The HTTP method in GetMethod_ not exists.");
    }
}

const std::string Methods::GetMethod_(EnumMethods& method)
{
    auto found = map_enum_methods_.find(method);
    if(found != map_enum_methods_.end())
        return found->second;
    else
        return HTTP_GET;
}

void Methods::AddProcess_(const std::string method, MethodProcess process)
{
    MethodStruct method_struct;
    method_struct.method = GetMethod_(method);
    method_struct.process = process;
    process_.push_back(method_struct);
}

bool Methods::EvaluateProcess_(std::string request_method)
{
    for(auto& process : process_)
    {
        if(request_method == GetMethod_(process.method))
        {
            process.process(*this);
            return true;
        }
    }

    return false;
}
std::string Methods::IdentifyMethod_(std::string method)
{
    for(auto method_it: map_string_methods_)
    {
        if(method == method_it.first)
            return method;
    }

    return HTTP_GET;
}

void Methods::MapMethods_()
{
    map_string_methods_.insert(std::pair{HTTP_GET, EnumMethods::kHTTP_GET});
    map_string_methods_.insert(std::pair{HTTP_POST, EnumMethods::kHTTP_POST});
    map_string_methods_.insert(std::pair{HTTP_PUT, EnumMethods::kHTTP_PUT});
    map_string_methods_.insert(std::pair{HTTP_DEL, EnumMethods::kHTTP_DEL});
    map_string_methods_.insert(std::pair{HTTP_OPTIONS, EnumMethods::kHTTP_OPTIONS});
    map_string_methods_.insert(std::pair{HTTP_HEAD, EnumMethods::kHTTP_HEAD});
    map_string_methods_.insert(std::pair{HTTP_PATCH, EnumMethods::kHTTP_PATCH});

    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_GET, HTTP_GET});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_POST, HTTP_POST});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_PUT, HTTP_PUT});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_DEL, HTTP_DEL});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_OPTIONS, HTTP_OPTIONS});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_HEAD, HTTP_HEAD});
    map_enum_methods_.insert(std::pair{EnumMethods::kHTTP_PATCH, HTTP_PATCH});
}