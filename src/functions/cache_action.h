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

#ifndef ATOM_FUNCTIONS_CACHEACTION
#define ATOM_FUNCTIONS_CACHEACTION


#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>

#include "functions/action.h"
#include "query/results.h"

#include "Poco/Exception.h"
#include "functions/action.h"


namespace Atom
{
    namespace Functions
    {
        class CacheAction;
    }
}

using namespace std::chrono_literals;


class Atom::Functions::CacheAction : public Functions::Action
{
    public:
        using Ptr = std::shared_ptr<CacheAction>;

        CacheAction(std::string identifier, std::chrono::seconds update_time);

        std::shared_ptr<std::thread> get_updates_thread() const { return updates_thread_; }
        bool get_state() const { return state_; }
        std::chrono::seconds get_update_time() const { return update_time_; }

        void set_state(bool state) { state_ = state; }
        void set_update_time(std::chrono::seconds update_time) { update_time_ = update_time; }

        virtual bool Work_() override;

    protected:
        void WorkInBackground_();

    private:
        std::shared_ptr<std::thread> updates_thread_;
        bool state_;
        std::chrono::seconds update_time_;
        std::mutex mutex_;
};

#endif // ATOM_FUNCTIONS_CACHEACTION
