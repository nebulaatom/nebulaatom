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

#ifndef CPW_TOOLS_MANAGEJSON_H
#define CPW_TOOLS_MANAGEJSON_H


#include <istream>
#include <string>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/StreamCopier.h>

using namespace Poco;


namespace CPW
{
	namespace Tools
	{
		class ManageJSON;
	}
}


class CPW::Tools::ManageJSON
{
	public:
		ManageJSON();
		~ManageJSON();

		JSON::Object::Ptr& get_json_body(){return json_body_;}

		std::string ReadBody_(std::istream& stream);
		bool Parse_(std::string string_to_parse);
		JSON::Object::Ptr ExtractObject_(Dynamic::Var& object);
		JSON::Array::Ptr ExtractArray_(Dynamic::Var object);

	protected:
		bool VerifyJSON_();

	private:
		JSON::Object::Ptr json_body_;
};

#endif // CPW_TOOLS_MANAGEJSON_H
