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

using namespace AtomTools;

ManageJSON::ManageJSON() :
    json_body_(new JSON::Array())
    ,json_body_string_("")
{

}

ManageJSON::~ManageJSON()
{

}

std::string ManageJSON::ReadBody_(std::istream& stream)
{
    std::string json_body;
    StreamCopier::copyToString(stream, json_body);

    if(json_body.empty())
        return "";

    return json_body;
}

bool ManageJSON::Parse_(std::string string_to_parse)
{
    if(string_to_parse.empty())
        return false;

    JSON::Parser parser;

    Dynamic::Var var_tmp = parser.parse(string_to_parse);

    if(!var_tmp.isArray())
        return false;

    json_body_ = var_tmp.extract<JSON::Array::Ptr>();
    json_body_string_ = string_to_parse;

    return true;
}

JSON::Object::Ptr ManageJSON::ExtractObject_(Dynamic::Var& object)
{
    if(object.isEmpty() || object.isArray())
        return JSON::Object::Ptr();

    return object.extract<JSON::Object::Ptr>();
}

JSON::Array::Ptr ManageJSON::ExtractArray_(Dynamic::Var object)
{
    auto final = JSON::Array::Ptr();

    if(object.isEmpty() || !object.isArray())
        return final;

    final = object.extract<JSON::Array::Ptr>();
    return final;
}
