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

#ifndef ATOM_CORE_NEBULAATOM
#define ATOM_CORE_NEBULAATOM


#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>

#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/NetSSL.h>
#include "Poco/Format.h"
#include "Poco/Exception.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include <Poco/Crypto/RSAKey.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/NetSSL.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Util/Application.h>

#include "core/handler_factory.h"
#include "query/database_manager.h"
#include "tools/settings_manager.h"
#include "tools/sessions_manager.h"


namespace Atom
{
    namespace Core
    {
        struct SSLInitializer;
        class NebulaAtom;
    }
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

class Atom::Core::NebulaAtom : public ServerApplication
{
    public:
        NebulaAtom(bool use_ssl = false);
        virtual ~NebulaAtom();

        bool get_use_ssl() const { return use_ssl_; }
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

        void set_use_ssl(bool use_ssl) { use_ssl_ = use_ssl; }

        void CustomHandlerCreator_(HandlerFactory::FunctionHandlerCreator handler_creator);
        void AddHandler_(std::string route, HandlerFactory::FunctionHandler handler);

    protected:
        virtual int main(const std::vector<std::string>& args);
        virtual void initialize(Application& self);
        virtual void uninitialize();
        int Init_();

    private:
        bool use_ssl_;
        std::unique_ptr<HTTPServer> server_;
        HandlerFactory* handler_factory_;
        Application& app_;
        const std::vector<std::string>* arguments_;
        Tools::SettingsManager settings_manager_;
};

#endif // ATOM_CORE_NEBULAATOM
