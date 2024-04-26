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

#ifndef ATOM_HTTPMETHODS
#define ATOM_HTTPMETHODS

#include <map>
#include <string>
#include <functional>
#include <list>
#include <exception>


namespace Atom
{
    namespace HTTP
    {
        const std::string HTTP_GET = "GET";
        const std::string HTTP_POST = "POST";
        const std::string HTTP_PUT = "PUT";
        const std::string HTTP_DEL = "DEL";
        const std::string HTTP_OPTIONS = "OPTIONS";
        const std::string HTTP_HEAD = "HEAD";
        const std::string HTTP_PATCH = "PATCH";
        enum class EnumMethods;
        class Methods;
    }
}


enum class Atom::HTTP::EnumMethods
{
    kHTTP_GET
    ,kHTTP_POST
    ,kHTTP_PUT
    ,kHTTP_DEL
    ,kHTTP_OPTIONS
    ,kHTTP_HEAD
    ,kHTTP_PATCH
    ,kNULL
};

class Atom::HTTP::Methods
{
    public:
        using MethodProcess = std::function<void(HTTP::Methods&)>;

        struct MethodStruct
        {
            HTTP::EnumMethods method;
            MethodProcess process;
        };

        Methods();
        virtual ~Methods(){}

        std::map<const std::string, EnumMethods>& get_map_string_methods()
        {
            auto& var = map_string_methods_;
            return var;
        }
        std::map<EnumMethods, const std::string>& get_map_enum_methods()
        {
            auto& var = map_enum_methods_;
            return var;
        }

        EnumMethods GetMethod_(const std::string method);
        const std::string GetMethod_(EnumMethods& method);
        void AddProcess_(const std::string method, MethodProcess process);
        bool EvaluateProcess_(std::string request_method);

    private:
        std::map<const std::string, EnumMethods> map_string_methods_;
        std::map<EnumMethods, const std::string> map_enum_methods_;
        std::list<MethodStruct> process_;
};

#endif // ATOM_HTTPMETHODS
