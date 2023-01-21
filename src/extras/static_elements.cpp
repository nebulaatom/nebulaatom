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

#include "extras/static_elements.h"

using namespace CPW::Extras;

StaticElements::StaticElements() :
    sessions_handler_(new Tools::SessionsHandler)
    ,database_manager_(new Query::DatabaseManager)
{
    try
    {
        database_manager_->StartMySQL_();
    }
    catch(std::exception& error)
    {
        std::cout << "- Error on static_elements.cc on StaticElements(): " << error.what() << std::endl;
    }
}

StaticElements::~StaticElements()
{
    database_manager_->StopMySQL_();
}
