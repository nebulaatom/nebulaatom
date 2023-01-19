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

#ifndef CPW_EXTRAS_STATIC_ELEMENTS_H
#define CPW_EXTRAS_STATIC_ELEMENTS_H


#include "tools/sessions_handler.h"
#include "query/database_manager.h"


namespace CPW
{
    namespace Extras
    {
        class StaticElements;
    }
}

using namespace CPW;


class CPW::Extras::StaticElements
{
    public:
        StaticElements();
        ~StaticElements();

        std::shared_ptr<Tools::SessionsHandler> get_sessions_handler() const
        {
            return sessions_handler_;
        }
        std::shared_ptr<Query::DatabaseManager> get_database_manager() const
        {
            return database_manager_;
        }

    private:
        std::shared_ptr<Tools::SessionsHandler> sessions_handler_;
        std::shared_ptr<Query::DatabaseManager> database_manager_;
};

#endif // CPW_EXTRAS_STATIC_ELEMENTS_H
