/*
* Nebula Atom

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

#include "core/server.h"

using namespace Atom::Core;

Server::Server(HTTPRequestHandlerFactory::Ptr factory, const ServerSocket& socket, HTTPServerParams::Ptr params) :
    Net::HTTPServer(factory, socket, params)
    ,server_name_("nebulaatom")
{
    SetupParams_(params);
}

Server::Server(HTTPRequestHandlerFactory::Ptr factory, const SecureServerSocket& socket, HTTPServerParams::Ptr params) :
    Net::HTTPServer(factory, socket, params)
    ,server_name_("nebulaatom")
{
    SetupParams_(params);
}

Server::~Server()
{

}

void Server::SetupParams_(HTTPServerParams::Ptr params)
{
    params->setServerName(server_name_);
    params->setMaxQueued(Tools::SettingsManager::get_basic_properties_().max_queued);
    params->setMaxThreads(Tools::SettingsManager::get_basic_properties_().max_threads);
}