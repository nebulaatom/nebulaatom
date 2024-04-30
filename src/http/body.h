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

#ifndef ATOM_HTTP_BODY
#define ATOM_HTTP_BODY


#include <istream>

#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/Net/MessageHeader.h>

#include "tools/manage_json.h"


namespace Atom
{
    namespace HTTP
    {
        class Body;
    }
}

using namespace Poco;


class Atom::HTTP::Body : public Tools::ManageJSON
{
    public:
        enum class Type
        {
            kFormURLEncoded
            ,kJSON
            ,kURI
        };

        using Ptr = std::shared_ptr<Body>;

        Body();
        ~Body(){}

        Type get_body_type() const { return body_type_; }
        URI::QueryParameters& get_query_parameters()
        {
            auto& var = query_parameters_;
            return var;
        }
        Net::NameValueCollection& get_parameters()
        {
            auto& var = parameters_;
            return var;
        }

        void ReadFormURLEncoded_(std::istream& stream);
        void ReadJSON_(std::istream& stream);
        void ReadFromURI_(std::string& uri);

    private:
        Type body_type_;
        URI::QueryParameters query_parameters_;
        Net::NameValueCollection parameters_;
        
};

#endif // ATOM_HTTP_BODY
