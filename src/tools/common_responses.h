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

#ifndef CPW_COMMONRESPONSES_H
#define CPW_COMMONRESPONSES_H


#include <string>
#include <map>

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>


namespace CPW
{
	namespace Tools
	{
		enum class ResponseType;
		class CommonResponses;
	}
}

using namespace Poco;
using namespace Poco::Net;


enum class CPW::Tools::ResponseType
{
	kError
	,kWarning
	,kInformation
	,kSuccess
};

class CPW::Tools::CommonResponses
{
	public:
		CommonResponses();
		~CommonResponses();

		std::map<HTTPResponse::HTTPStatus, std::pair<ResponseType, std::string>>& get_responses_()
		{
			auto& var = responses_;
			return var;
		}

		void GenericResponse_(HTTPServerResponse& response, HTTPResponse::HTTPStatus status, std::string message);

	protected:
		void FillResponses_();

	private:
		std::map<HTTPResponse::HTTPStatus, std::pair<ResponseType, std::string>> responses_;
};

#endif // CPW_COMMONRESPONSES_H
