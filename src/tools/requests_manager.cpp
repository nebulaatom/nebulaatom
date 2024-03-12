/*
* <one line to give the program's name and a brief idea of what it does.>
* Copyright (C) 2021  <copyright holder> <email>
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

#include "tools/requests_manager.h"

using namespace AtomTools;

RequestsManager::RequestsManager()
{
    PrepareMethods_();
}

RequestsManager::~RequestsManager()
{

}

void RequestsManager::PrepareMethods_()
{
    actions_strings_.emplace(std::make_pair
    (
        "GET"
        ,[&](){http_methods_->HandleGETMethod_();}
    ));
    actions_strings_.emplace(std::make_pair
    (
        "POST"
        ,[&](){http_methods_->HandlePOSTMethod_();}
    ));
    actions_strings_.emplace(std::make_pair
    (
        "PUT"
        ,[&](){http_methods_->HandlePUTMethod_();}
    ));
    actions_strings_.emplace(std::make_pair
    (
        "DEL"
        ,[&](){http_methods_->HandleDELMethod_();}
    ));
}
