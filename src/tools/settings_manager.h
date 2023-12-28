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

#ifndef CPW_TOOLS_SETTINGS_MANAGER_H
#define CPW_TOOLS_SETTINGS_MANAGER_H


#include <iostream>
#include <memory>
#include <string>

#include "yaml-cpp/yaml.h"
#include "Poco/Exception.h"
#include "Poco/Util/ServerApplication.h"

#include "functions/functions_manager.h"


namespace CPW
{
    namespace Tools
    {
        class SettingsManager;
    }
}

using namespace Poco;
using namespace Poco::Util;


class CPW::Tools::SettingsManager
{
    public:
        SettingsManager();

        Functions::FunctionsManager& get_functions_manager()
        {
            auto& var = functions_manager_;
            return var;
        }

        static void ReadFunctions_();

    private:
        static Functions::FunctionsManager functions_manager_;
};

#endif // CPW_TOOLS_SETTINGS_MANAGER_H
