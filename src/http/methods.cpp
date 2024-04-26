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

using namespace Atom::HTTP;

Methods::Methods()
{

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