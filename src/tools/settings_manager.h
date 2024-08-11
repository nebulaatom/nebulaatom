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

#ifndef NAF_TOOLS_SETTINGSMANAGER
#define NAF_TOOLS_SETTINGSMANAGER


#include <iostream>
#include <memory>
#include <string>
#include <mutex>

#include "yaml-cpp/node/detail/iterator_fwd.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/yaml.h"
#include "Poco/Exception.h"

#include "tools/output_logger.h"

namespace NAF
{
    namespace Tools
    {
        class SettingsManager;
    }
}

using namespace Poco;


class NAF::Tools::SettingsManager
{
    public:
        struct BasicProperties
        {
            int port, max_queued, max_threads, session_max_age;
            float max_file_size;
            std::string db_host, db_port, db_name, db_user, db_password;
            std::string directory_base, directory_for_temp_files;
            std::string certificate, key, rootcert;
            std::string logger_output_file;
            bool debug;
        };

        SettingsManager();

        static BasicProperties& get_basic_properties_()
        {
            auto& var = basic_properties_;
            return var;
        }
        static std::string get_properties_file_address() { return properties_file_address_; }

        static void set_properties_file_address(std::string properties_file_address) { properties_file_address_ = properties_file_address; }

        static void SetUpProperties_();
        static void ReadBasicProperties_();

    protected:
        static bool VerifyYAMLScalarNode_(YAML::Node& node);
        static bool VerifyYAMLMapNode_(YAML::Node& node);
        static void PrintError_(std::string function, std::string variable);

    private:
        static std::mutex mutex_;
        static BasicProperties basic_properties_;
        static std::string properties_file_address_;
};

#endif // NAF_TOOLS_SETTINGSMANAGER
