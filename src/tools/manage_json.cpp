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

#include "tools/manage_json.h"

using namespace Atom::Tools;

ManageJSON::ManageJSON() :
    json_array_(new JSON::Array())
    ,json_object_(new JSON::Object())
{

}

ManageJSON::~ManageJSON()
{

}

void ManageJSON::Parse_(std::string& string_to_parse)
{
    JSON::Parser parser;

    Dynamic::Var var_tmp(parser.parse(string_to_parse));

    if(var_tmp.isArray())
        json_array_ = var_tmp.extract<JSON::Array::Ptr>();
    else
        json_object_ = var_tmp.extract<JSON::Object::Ptr>();
}

JSON::Object::Ptr ManageJSON::ExtractObject_(Dynamic::Var& object)
{
    if(object.isEmpty() || object.isArray())
        return JSON::Object::Ptr();

    return object.extract<JSON::Object::Ptr>();
}

JSON::Array::Ptr ManageJSON::ExtractArray_(Dynamic::Var& object)
{
    auto final = JSON::Array::Ptr();

    if(object.isEmpty() || !object.isArray())
        return final;

    final = object.extract<JSON::Array::Ptr>();
    return final;
}
