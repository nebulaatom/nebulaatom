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

#include "tools/sessions_handler.h"

using namespace CPW::Tools;

std::map<std::string, CPW::Extras::Session> SessionsHandler::sessions_ = {};

SessionsHandler::SessionsHandler()
{

}

SessionsHandler::~SessionsHandler()
{

}

CPW::Extras::Session& SessionsHandler::CreateSession_(std::string user, std::string path, int max_age)
{
    Extras::Session new_session;
    new_session.set_user(user);
    new_session.set_path(path);
    new_session.set_max_age(max_age);

    std::string id;
    while(true)
    {
        id = new_session.get_id();
        if(SessionExists_(id))
            new_session.GenerateNewSessionID_();
        else
            break;
    }

    sessions_.emplace(id, std::move(new_session));
    return sessions_.at(id);
}

void SessionsHandler::DeleteSession_(std::string id)
{
    if(sessions_.find(id) != sessions_.end())
        sessions_.erase(id);
}

bool SessionsHandler::SessionExists_(std::string id)
{
    return sessions_.find(id) != sessions_.end();
}
