/*
 * object_library.h
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

#ifndef OBJECT_LIBRARY_H
#define OBJECT_LIBRARY_H


#include <iostream>
#include <string>


class ObjectLibrary
{
	public:
		ObjectLibrary();
		virtual ~ObjectLibrary();
		
		std::string SayHello_();
	
	private:
		/* add your private declarations */
};

#endif /* OBJECT_LIBRARY_H */ 
