/*
* Nebula Atom
* Copyright (C) 2021 NebulaCSF support@cpwonline.org
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

#ifndef ATOMCORE_WOODPECKERSERVER_H
#define ATOMCORE_WOODPECKERSERVER_H


#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"

#include "core/handler_factory.h"
#include "query/database_manager.h"
#include "tools/settings_manager.h"
#include "tools/sessions_manager.h"


namespace CPW
{
    namespace Core
    {
        class WoodpeckerServer;
    }
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


class AtomCore::WoodpeckerServer : public ServerApplication
{
    public:
        WoodpeckerServer();
        virtual ~WoodpeckerServer();

        HTTPServerParams::Ptr get_server_params()
        {
            auto& var = server_params_;
            return var;
        }
        ServerSocket* get_server_socket()
        {
            auto var = server_socket_.get();
            return var;
        }
        SecureServerSocket* get_secure_server_socket()
        {
            auto var = secure_server_socket_.get();
            return var;
        }
        HTTPServer* get_server()
        {
            auto var = server_.get();
            return var;
        }
        HandlerFactory* get_handler_factory()
        {
            auto& var = handler_factory_;
            return var;
        }

        void CustomHandlerCreator_(HandlerFactory::FunctionHandlerCreator handler_creator);
        void AddHandler_(std::string route, HandlerFactory::FunctionHandler handler);

    protected:
        virtual int main(const std::vector<std::string>& args);
        virtual void initialize(Application& self);
        virtual void uninitialize();
        int Init_();

    private:
        HTTPServerParams::Ptr server_params_;
        std::shared_ptr<ServerSocket> server_socket_;
        std::shared_ptr<SecureServerSocket> secure_server_socket_;
        std::unique_ptr<HTTPServer> server_;
        HandlerFactory* handler_factory_;
        Application& app_;
        const std::vector<std::string>* arguments_;
        Tools::SettingsManager settings_manager_;
};

#endif // ATOMCORE_WOODPECKERSERVER_H
