/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2023  Jose F Rivas C <email>
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

#ifndef ATOM_TOOLS_SETTINGS_MANAGER
#define ATOM_TOOLS_SETTINGS_MANAGER


#include <iostream>
#include <memory>
#include <string>

#include "yaml-cpp/node/detail/iterator_fwd.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/yaml.h"
#include "Poco/Exception.h"
#include "Poco/Util/ServerApplication.h"



namespace Atom
{
    namespace Tools
    {
        class SettingsManager;
    }
}

using namespace Poco;
using namespace Poco::Util;


class Atom::Tools::SettingsManager
{
    public:
        struct BasicProperties
        {
            int port, max_queued, max_threads, session_max_age;
            std::string db_host, db_port, db_name, db_user, db_password;
            std::string directory_base, directory_for_uploaded_files, directory_for_temp_files;
            std::string certificate, key, rootcert;
        };

        SettingsManager();

        BasicProperties& get_basic_properties_()
        {
            auto& var = basic_properties_;
            return var;
        }

        static void SetUpProperties_();
        static void ReadBasicProperties_();

    protected:
        static bool VerifyYAMLScalarNode_(YAML::Node& node);
        static bool VerifyYAMLMapNode_(YAML::Node& node);
        static void PrintError_(std::string function, std::string variable);

    private:
        static BasicProperties basic_properties_;
};

#endif // ATOM_TOOLS_SETTINGS_MANAGER
