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

#ifndef CPW_HTTP_SESSIONS_MANAGER_H
#define CPW_HTTP_SESSIONS_MANAGER_H


#include <string>
#include <map>

#include "extras/session.h"
#include "functions/sql_action.h"

namespace CPW
{
    namespace Tools
    {
        class SessionsManager;
    }
}


class CPW::Tools::SessionsManager
{
    public:
        SessionsManager();
        ~SessionsManager();

        static std::map<std::string, CPW::Extras::Session>& get_sessions()
        {
            auto& var = sessions_;
            return var;
        }

        static void ReadSessions_();
        static CPW::Extras::Session& CreateSession_(std::string user, std::string path, int max_age);
        static void DeleteSession_(std::string id);

    protected:
        static bool SessionExists_(std::string id);

    private:
        static std::map<std::string, CPW::Extras::Session> sessions_;
};

#endif // CPW_HTTP_SESSIONS_MANAGER_H
