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

#ifndef CPW_HANDLERS_FRONTENDHANDLER_H
#define CPW_HANDLERS_FRONTENDHANDLER_H

#include <string>
#include <map>
#include <vector>

#include "handlers/root_handler.h"
#include "tools/file_manager.h"


namespace CPW
{
	namespace Handlers
	{
		class FrontendHandler;
	}
}


class CPW::Handlers::FrontendHandler : public RootHandler
{
	public:
		FrontendHandler(std::string api_version);
		~FrontendHandler();

	protected:
		virtual void HandleGETMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandlePOSTMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandlePUTMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void HandleDELMethod_(HTTPServerRequest& request, HTTPServerResponse& response) override;
		virtual void AddRoutes_() override;

	private:
		Tools::FileManager file_manager_;
};

#endif // CPW_HANDLERS_FRONTENDHANDLER_H
