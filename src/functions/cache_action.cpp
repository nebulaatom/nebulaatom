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

#include "functions/cache_action.h"

using namespace NAF::Functions;

CacheAction::CacheAction(std::string identifier, std::chrono::seconds update_time) :
    Functions::Action(identifier)
    ,state_(true)
    ,update_time_(update_time)
{

}

bool CacheAction::Work_()
{
    try
    {
        updates_thread_ = std::make_shared<std::thread>(&CacheAction::WorkInBackground_, this);
    }
    catch(std::runtime_error& error)
    {
        NotifyError_("Error on cache_action.cpp on Work_(): " + std::string(error.what()));
        return false;
    }
    catch(std::exception& error)
    {
        NotifyError_("Error on cache_action.cpp on Work_(): " + std::string(error.what()));
        return false;
    }
    return false;
}

void CacheAction::WorkInBackground_()
{
    while(state_)
    {
        mutex_.lock();
        Tools::OutputLogger::Debug_("Work in background");
        get_results()->clear();
        Action::Work_();
        Tools::OutputLogger::Debug_("Work maded");
        mutex_.unlock();

        std::this_thread::sleep_for(update_time_);
    }
}