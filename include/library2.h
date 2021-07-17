/*
 * library2.h
 * 
 * Copyright 2021 CPW Online <support@cpwonline.org>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#ifndef LIBRARY2_H
#define LIBRARY2_H


#include <string>

#include "object_library.h"


class Library2
{
	public:
		/**
		 * @brief ...
		 *
		 */
		Library2();
		virtual ~Library2();
		
		std::string get_var1() const;
	
	private:
		std::string var1_;
};

#endif /* LIBRARY2_H */ 
