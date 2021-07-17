/*
 * main.cc
 * 
 * Copyright 2021 Jose Felix Rivas Carrasco <josefelixrc@outlook.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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


#include <iostream>

 #include "Poco/MD5Engine.h"
 #include "Poco/DigestStream.h"
 
#include "project-nameConfig.h"
#include "library1.h"
#include "library2.h"

int main(int argc, char* argv[])
{
	std::cout << "\n This project is: " << PROJECT_NAME;
	
	Library1 obj1;
	Library2 obj2;
	
	std::cout << "\nThis hello on library2: " << obj2.get_var1();
	
	// Using POCO:
	Poco::MD5Engine md5;
	Poco::DigestOutputStream ds(md5);
	std::string str = {};
	if( argc > 1 )
	{
		str = argv[1];
	}
	else
	{
		std::cout << "\nEnter the word to generate the HASH MD5 ";
		std::cin >> str;
	}
	ds << str;
	ds.close();
	std::cout << "\n" << Poco::DigestEngine::digestToHex(md5.digest()) << '\n';

	return 0;
}
