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

#include "tools/settings_manager.h"

using namespace NAF;
using namespace Tools;

std::mutex SettingsManager::mutex_;
std::vector<Tools::SettingsManager::Setting> Tools::SettingsManager::settings_ = {};
std::string Tools::SettingsManager::config_path_ = "properties.yaml";

Tools::SettingsManager::SettingsManager()
{
    
}

std::vector<SettingsManager::Setting>::iterator SettingsManager::GetSetting_(std::string setting_name)
{
    auto found = std::find_if(settings_.begin(), settings_.end(), [&setting_name](Setting& setting)
    {
        return setting.name == setting_name;
    });
    return found;
}

std::string SettingsManager::GetSetting_(std::string setting_name, const char* another_value)
{
    auto setting = GetSetting_(setting_name);
    if(setting == settings_.end())
        return another_value;
    else if(setting->type != Tools::DValue::Type::kString)
        return another_value;

    return setting->value.String_();
}

std::string SettingsManager::GetSetting_(std::string setting_name, std::string another_value)
{
    auto setting = GetSetting_(setting_name);
    if(setting == settings_.end())
        return another_value;
    else if(setting->type != Tools::DValue::Type::kString)
        return another_value;

    return setting->value.String_();
}

int SettingsManager::GetSetting_(std::string setting_name, int another_value)
{
    auto setting = GetSetting_(setting_name);
    if(setting == settings_.end())
        return another_value;
    else if(setting->type != Tools::DValue::Type::kInteger)
        return another_value;

    return setting->value.Int_();
}

float SettingsManager::GetSetting_(std::string setting_name, float another_value)
{
    auto setting = GetSetting_(setting_name);
    if(setting == settings_.end())
        return another_value;
    else if(setting->type != Tools::DValue::Type::kFloat)
        return another_value;

    return setting->value.Float_();
}

bool SettingsManager::GetSetting_(std::string setting_name, bool another_value)
{
    auto setting = GetSetting_(setting_name);
    if(setting == settings_.end())
        return another_value;
    else if(setting->type != Tools::DValue::Type::kBoolean)
        return another_value;

    return setting->value.Bool_();
}

void Tools::SettingsManager::AddSetting_(std::string name, Tools::DValue::Type type, Tools::DValue value)
{
    settings_.push_back(Setting(name, type, value));
}

void Tools::SettingsManager::ReadSettings_()
{
    try
    {
        mutex_.lock();
        // Read YAML functions file
        YAML::Node config = YAML::LoadFile(config_path_);

        for(auto& setting : settings_)
        {
            try
            {
                auto tmp_val = config[setting.name];
                if (!VerifyYAMLScalarNode_(tmp_val))
                    continue;

                switch(setting.type)
                {
                    case Tools::DValue::Type::kEmpty:
                    {
                        break;
                    }
                    case Tools::DValue::Type::kBoolean:
                    {
                        setting.value = Tools::DValue(tmp_val.as<bool>());
                        break;
                    }
                    case Tools::DValue::Type::kFloat:
                    {
                        setting.value = Tools::DValue(tmp_val.as<float>());
                        break;
                    }
                    case Tools::DValue::Type::kInteger:
                    {
                        setting.value = Tools::DValue(tmp_val.as<int>());
                        break;
                    }
                    case Tools::DValue::Type::kString:
                    {
                        setting.value = Tools::DValue(tmp_val.as<std::string>());
                        break;
                    }
                }
            }
            catch(std::exception& e)
            {
                PrintError_("ReadSettings_", setting.name + ": " + e.what());
            }
        }

        mutex_.unlock();
    }
    catch(std::exception& e)
    {
        PrintError_("ReadSettings_", std::string(e.what()));
        return;
    }
}

bool Tools::SettingsManager::VerifyYAMLScalarNode_(YAML::Node& node)
{
    if(!node || !node.IsScalar())
        return false;
    else
        return true;
}

void Tools::SettingsManager::PrintError_(std::string function, std::string variable)
{
    std::cerr << "Error on settings_manager.cpp on " << function << "(): The yaml file is malformed. ERRYML" << function << "(" << variable << ")" << std::endl;
}

bool Tools::SettingsManager::VerifyYAMLMapNode_(YAML::Node& node)
{
    if(!node || !node.IsMap())
        return false;
    else
        return true;
}