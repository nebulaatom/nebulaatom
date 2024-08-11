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
Tools::SettingsManager::BasicProperties Tools::SettingsManager::basic_properties_ = {};
std::string Tools::SettingsManager::properties_file_address_ = "properties.yaml";

Tools::SettingsManager::SettingsManager()
{
    
}

void Tools::SettingsManager::SetUpProperties_()
{
    mutex_.lock();
    basic_properties_.port = 8080;
    basic_properties_.max_queued = 1000;
    basic_properties_.max_threads = 16;
    basic_properties_.max_file_size = 15;
    basic_properties_.db_host = "127.0.0.1";
    basic_properties_.db_port = "3306";
    basic_properties_.db_name = "";
    basic_properties_.db_user = "";
    basic_properties_.db_password = "";
    basic_properties_.session_max_age = 3600;
    basic_properties_.directory_base = "/var/www";
    basic_properties_.directory_for_temp_files = "/tmp";
    basic_properties_.certificate = "";
    basic_properties_.key = "";
    basic_properties_.rootcert = "";
    basic_properties_.logger_output_file = "nebulaatom.log";
    basic_properties_.debug = false;
    mutex_.unlock();
}

void Tools::SettingsManager::ReadBasicProperties_()
{
    try
    {
        mutex_.lock();
        // Read YAML functions file
        YAML::Node config = YAML::LoadFile(properties_file_address_);

        // port
        auto port = config["port"];
        if (!VerifyYAMLScalarNode_(port))
            PrintError_("ReadBasicProperties_", "port");

        basic_properties_.port = port.as<int>();
        
        // max_queued
        auto max_queued = config["max_queued"];
        if (!VerifyYAMLScalarNode_(max_queued))
            PrintError_("ReadBasicProperties_", "max_queued");
            
        basic_properties_.max_queued = max_queued.as<int>();
        
        // max_threads
        auto max_threads = config["max_threads"];
        if (!VerifyYAMLScalarNode_(max_threads))
            PrintError_("ReadBasicProperties_", "max_threads");
            
        basic_properties_.max_threads = max_threads.as<int>();
        
        // session_max_age
        auto session_max_age = config["session_max_age"];
        if (!VerifyYAMLScalarNode_(session_max_age))
            PrintError_("ReadBasicProperties_", "session_max_age");
            
        basic_properties_.session_max_age = session_max_age.as<int>();
        
        // max_file_size
        auto max_file_size = config["max_file_size"];
        if (!VerifyYAMLScalarNode_(max_file_size))
            PrintError_("ReadBasicProperties_", "max_file_size");
            
        basic_properties_.max_file_size = max_file_size.as<float>();
        
        // directory_base
        auto directory_base = config["directory_base"];
        if (!VerifyYAMLScalarNode_(directory_base))
            PrintError_("ReadBasicProperties_", "directory_base");

        basic_properties_.directory_base = directory_base.as<std::string>();
        
        // directory_for_temp_files
        auto directory_for_temp_files = config["directory_for_temp_files"];
        if (!VerifyYAMLScalarNode_(directory_for_temp_files))
            PrintError_("ReadBasicProperties_", "directory_for_temp_files");
            
        basic_properties_.directory_for_temp_files = directory_for_temp_files.as<std::string>();
        
        // db_host
        auto db_host = config["db_host"];
        if (!VerifyYAMLScalarNode_(db_host))
            PrintError_("ReadBasicProperties_", "db_host");

        basic_properties_.db_host = db_host.as<std::string>();
        
        // db_port
        auto db_port = config["db_port"];
        if (!VerifyYAMLScalarNode_(db_port))
            PrintError_("ReadBasicProperties_", "db_port");
            
        basic_properties_.db_port = db_port.as<std::string>();
        
        // db_name
        auto db_name = config["db_name"];
        if (!VerifyYAMLScalarNode_(db_name))
            PrintError_("ReadBasicProperties_", "db_name");
            
        basic_properties_.db_name = db_name.as<std::string>();
        
        // db_user
        auto db_user = config["db_user"];
        if (!VerifyYAMLScalarNode_(db_user))
            PrintError_("ReadBasicProperties_", "db_user");
            
        basic_properties_.db_user = db_user.as<std::string>();
        
        // db_password
        auto db_password = config["db_password"];
        if (!VerifyYAMLScalarNode_(db_password))
            PrintError_("ReadBasicProperties_", "db_password");
            
        basic_properties_.db_password = db_password.as<std::string>();
        
        // certificate
        auto certificate = config["certificate"];
        if (!VerifyYAMLScalarNode_(certificate))
            PrintError_("ReadBasicProperties_", "certificate");
            
        basic_properties_.certificate = certificate.as<std::string>();
        
        // key
        auto key = config["key"];
        if (!VerifyYAMLScalarNode_(key))
            PrintError_("ReadBasicProperties_", "key");
            
        basic_properties_.key = key.as<std::string>();
        
        // rootcert
        auto rootcert = config["rootcert"];
        if (!VerifyYAMLScalarNode_(rootcert))
            PrintError_("ReadBasicProperties_", "rootcert");
            
        basic_properties_.rootcert = rootcert.as<std::string>();
        
        // logger_output_file_
        auto logger_output_file = config["logger_output_file"];
        if (!VerifyYAMLScalarNode_(logger_output_file))
            PrintError_("ReadBasicProperties_", "logger_output_file");
        
        basic_properties_.logger_output_file = logger_output_file.as<std::string>();
        Tools::OutputLogger::set_output_file_address(basic_properties_.logger_output_file);
        
        // debug
        auto debug = config["debug"];
        if (!VerifyYAMLScalarNode_(debug))
            PrintError_("ReadBasicProperties_", "debug");
        
        basic_properties_.debug = debug.as<bool>();
        Tools::OutputLogger::set_print_debug(basic_properties_.debug);
        
        
        mutex_.unlock();
    }
    catch(std::exception& e)
    {
        PrintError_("ReadBasicProperties_", std::string(e.what()));
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