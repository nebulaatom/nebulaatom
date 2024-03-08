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

#include "woodpecker_server.h"

using namespace CPW::Core;

WoodpeckerServer::WoodpeckerServer() :
    server_params_(new HTTPServerParams())
    ,handler_factory_(new HandlerFactory())
    ,app_(Application::instance())
{
    //Poco::Net::initializeSSL();
    //Tools::SettingsManager::ReadBasicProperties_();
    //Query::DatabaseManager::StartMySQL_();
    //Tools::SettingsManager::ReadFunctions_();
    //Tools::SessionsManager::ReadSessions_();
}

WoodpeckerServer::~WoodpeckerServer()
{
    //Poco::Net::uninitializeSSL();
    //Query::DatabaseManager::StopMySQL_();
}

void WoodpeckerServer::initialize(Application& self)
{
    loadConfiguration();
    ServerApplication::initialize(self);
}

void WoodpeckerServer::uninitialize()
{
    ServerApplication::uninitialize();
}

int WoodpeckerServer::main(const std::vector<std::string>& args)
{
    try
    {
        arguments_ = &args;
        server_params_->setMaxQueued(settings_manager_.get_basic_properties_().max_queued);
        server_params_->setMaxThreads(settings_manager_.get_basic_properties_().max_threads);

        //server_socket_ = std::make_shared<SecureServerSocket>(settings_manager_.get_basic_properties_().port);
        server_socket_ = std::make_shared<ServerSocket>(settings_manager_.get_basic_properties_().port);
        server_ = std::make_unique<HTTPServer>(handler_factory_, *server_socket_.get(), server_params_);

        return Init_();
    }
    catch(MySQL::MySQLException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(JSON::JSONException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(Poco::NullPointerException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(Poco::SystemException& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + error.displayText());
        return -1;
    }
    catch(std::runtime_error& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
        return -1;
    }
    catch(std::exception& error)
    {
        app_.logger().error("- Error on handler_factory.cc on createRequestHandler(): " + std::string(error.what()));
        return -1;
    }

    return -1;
}

int WoodpeckerServer::Init_()
{
    server_->start();
    app_.logger().information("- Server started at port " + format("%d", settings_manager_.get_basic_properties_().port));

    waitForTerminationRequest();

    app_.logger().information("- Shutting down server... ");
    server_->stop();

    return Application::EXIT_OK;
}
