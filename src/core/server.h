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

#ifndef ATOM_CORE_SERVER
#define ATOM_CORE_SERVER


#include <Poco/ThreadPool.h>
#include <map>
#include <exception>
#include <vector>
#include <memory>
#include <stdexcept>

#include "Poco/Exception.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SecureServerSocket.h"

#include "core/handler_factory.h"
#include "tools/output_logger.h"
#include "tools/settings_manager.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

namespace Atom
{
    namespace Core
    {
        class Server;
    }
}


class Atom::Core::Server : public Util::ServerApplication
{
    public:
        using Ptr = std::shared_ptr<Server>;

        Server(HandlerFactory* handler_factory);

        bool get_use_ssl() const { return use_ssl_; }

        void set_use_ssl(bool use_ssl){ use_ssl_ = use_ssl; }

        void initialize(Application& self) override;
        void uninitialize() override;
        int main(const std::vector<std::string>& args) override;

    protected:
        void SetupParams_(HTTPServerParams::Ptr params);

    private:
        bool use_ssl_;
        std::string server_name_;
        HandlerFactory* handler_factory_;
        std::unique_ptr<HTTPServer> http_server_;
};

#endif // ATOM_CORE_SERVER
