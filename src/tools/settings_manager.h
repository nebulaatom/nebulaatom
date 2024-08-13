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
#include <algorithm>

#include "tools/dvalue.h"
#include "yaml-cpp/node/detail/iterator_fwd.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/yaml.h"
#include "Poco/Exception.h"

#include "tools/dvalue.h"

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
        struct Setting
        {
            Setting(std::string name, Tools::DValue::Type type, Tools::DValue value) :
                name(name)
                ,type(type)
                ,value(value)
            {

            }
            std::string name;
            Tools::DValue::Type type;
            Tools::DValue value;
        };

        SettingsManager();

        static std::vector<Setting>& get_settings()
        {
            auto& var = settings_;
            return var;
        }
        static std::string get_config_path() { return config_path_; }

        static void set_config_path(std::string config_path) { config_path_ = config_path; }

        static std::vector<Setting>::iterator GetSetting_(std::string setting_name);
        static std::string GetSetting_(std::string setting_name, const char* another_value);
        static std::string GetSetting_(std::string setting_name, std::string another_value);
        static int GetSetting_(std::string setting_name, int another_value);
        static float GetSetting_(std::string setting_name, float another_value);
        static bool GetSetting_(std::string setting_name, bool another_value);
        static void AddSetting_(std::string name, Tools::DValue::Type type, Tools::DValue value);
        static void AddBasicSettings_();
        static void ReadSettings_();

    protected:
        static bool VerifyYAMLScalarNode_(YAML::Node& node);
        static bool VerifyYAMLMapNode_(YAML::Node& node);
        static void PrintError_(std::string function, std::string variable);

    private:
        static std::mutex mutex_;
        static std::vector<Setting> settings_;
        static std::string config_path_;
};

#endif // NAF_TOOLS_SETTINGSMANAGER
