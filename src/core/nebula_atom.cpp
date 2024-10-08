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

#include "nebula_atom.h"
#include "query/database_manager.h"
#include "security/permissions_manager.h"
#include "tools/sessions_manager.h"

using namespace NAF::Core;

NebulaAtom::NebulaAtom(bool use_ssl) :
    use_ssl_(use_ssl)
    ,handler_factory_(new HandlerFactory())
{
    if(use_ssl_)
        Net::initializeSSL();

    AddBasicSettings_();
    SetupSettings_();
}

NebulaAtom::~NebulaAtom()
{
    if(use_ssl_)
        Net::uninitializeSSL();
}

void NebulaAtom::CustomHandlerCreator_(HandlerFactory::FunctionHandlerCreator handler_creator)
{
    handler_factory_->set_handler_creator(std::move(handler_creator));
}

void NebulaAtom::AddHandler_(std::string route, HandlerFactory::FunctionHandler handler)
{
    std::vector<std::string> segments;
    URI(route).getPathSegments(segments);
    Tools::Route requested_route(segments);

    handler_factory_->get_connections().insert({route, Tools::HandlerConnection{requested_route, handler}});
}

int NebulaAtom::Init_()
{
    try
    {
        server_ = std::make_shared<Server>(handler_factory_);
        server_->set_use_ssl(use_ssl_);
        return server_->run(console_parameters_);
    }
    catch(Net::SSLException& error)
    {
        Tools::OutputLogger::Error_("Error on nebula_atom.cpp on Init_(): " + error.displayText());
        return 1;
    }
    catch(Net::NetException& error)
    {
        Tools::OutputLogger::Error_("Error on nebula_atom.cpp on Init_(): " + error.displayText());
        return 1;
    }
    catch(Poco::NullPointerException& error)
    {
        Tools::OutputLogger::Error_("Error on nebula_atom.cpp on Init_(): " + error.displayText());
        return 1;
    }
    catch(Poco::SystemException& error)
    {
        Tools::OutputLogger::Error_("Error on nebula_atom.cpp on Init_(): " + error.displayText());
        return 1;
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Error_("Error on nebula_atom.cpp on Init_(): " + std::string(error.what()));
        return 1;
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on nebula_atom.cpp on Init_(): " + std::string(error.what()));
        return 1;
    }
    catch(...)
    {
        Tools::OutputLogger::Error_("Error on nebula_atom.cpp on Init_(): Unhandled");
        return 1;
    }
}

int NebulaAtom::Init_(int argc, char** argv)
{
    console_parameters_ = std::vector<std::string>(argv, argv + argc);

    return Init_();
}

void NebulaAtom::AddBasicSettings_()
{
    Tools::SettingsManager::AddSetting_("port", Tools::DValue::Type::kInteger, Tools::DValue(8080));
    Tools::SettingsManager::AddSetting_("max_queued", Tools::DValue::Type::kInteger, Tools::DValue(1000));
    Tools::SettingsManager::AddSetting_("max_threads", Tools::DValue::Type::kInteger, Tools::DValue(16));
    Tools::SettingsManager::AddSetting_("max_file_size", Tools::DValue::Type::kInteger, Tools::DValue(15));
    Tools::SettingsManager::AddSetting_("db_host", Tools::DValue::Type::kString, Tools::DValue("127.0.0.1"));
    Tools::SettingsManager::AddSetting_("db_port", Tools::DValue::Type::kString, Tools::DValue("3306"));
    Tools::SettingsManager::AddSetting_("db_name", Tools::DValue::Type::kString, Tools::DValue(""));
    Tools::SettingsManager::AddSetting_("db_user", Tools::DValue::Type::kString, Tools::DValue(""));
    Tools::SettingsManager::AddSetting_("db_password", Tools::DValue::Type::kString, Tools::DValue(""));
    Tools::SettingsManager::AddSetting_("session_max_age", Tools::DValue::Type::kInteger, Tools::DValue(3600));
    Tools::SettingsManager::AddSetting_("directory_base", Tools::DValue::Type::kString, Tools::DValue("/var/www"));
    Tools::SettingsManager::AddSetting_("directory_for_temp_files", Tools::DValue::Type::kString, Tools::DValue("/tmp"));
    Tools::SettingsManager::AddSetting_("certificate", Tools::DValue::Type::kString, Tools::DValue(""));
    Tools::SettingsManager::AddSetting_("key", Tools::DValue::Type::kString, Tools::DValue(""));
    Tools::SettingsManager::AddSetting_("rootcert", Tools::DValue::Type::kString, Tools::DValue(""));
    Tools::SettingsManager::AddSetting_("logger_output_file", Tools::DValue::Type::kString, Tools::DValue("nebulaatom.log"));
    Tools::SettingsManager::AddSetting_("debug", Tools::DValue::Type::kBoolean, Tools::DValue(false));
}

void NebulaAtom::SetupSettings_()
{
    Tools::OutputLogger::set_output_file_address(Tools::SettingsManager::GetSetting_("logger_output_file", "nebulaatom.log"));
    Tools::OutputLogger::set_print_debug(Tools::SettingsManager::GetSetting_("debug", true));
    Query::DatabaseManager::Credentials credentials(
        Tools::SettingsManager::GetSetting_("db_host", "localhost")
        ,Tools::SettingsManager::GetSetting_("db_port", "3306")
        ,Tools::SettingsManager::GetSetting_("db_name", "db")
        ,Tools::SettingsManager::GetSetting_("db_user", "root")
        ,Tools::SettingsManager::GetSetting_("db_password", "root")
    );
    Tools::SessionsManager::get_credentials().Replace_(credentials);
    Security::PermissionsManager::get_credentials().Replace_(credentials);
}