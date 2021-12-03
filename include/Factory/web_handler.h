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

#ifndef CPW_FACTORY_WEBHANDLER_H
#define CPW_FACTORY_WEBHANDLER_H

#include <string>
#include <map>
#include <vector>

#include "Poco/Path.h"
#include "Poco/File.h"

#include "Factory/root_handler.h"


namespace CPW
{
	namespace Factory
	{
		class WebHandler;
	}
}


class CPW::Factory::WebHandler : public RootHandler
{
	public:
		struct FileProperties
		{
			std::string content_type;
			bool binary;
			std::vector<std::string> other_extensions;
		};

		WebHandler(std::string api_version);
		~WebHandler();

	protected:
		virtual void HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void AddRoutes_() override;

		void AddSupportedFiles_();
		bool IsSupported_(std::string path);
		bool CheckFile_(std::string path);

	private:
		std::map<std::string, FileProperties>* supported_files_;;
};

#endif // CPW_FACTORY_WEBHANDLER_H
