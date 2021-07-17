/*
 * library1.cc
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


#include "library1.h"

Library1::Library1()
{
	auto app = Gtk::Application::create("org.gtkmm.examples.base");
	auto refBuilder = Gtk::Builder::create();
	try
	{
		refBuilder->add_from_file(DATADIR"project-name.glade");
	}
	catch(const Glib::FileError& exp)
	{
		std::cerr << "FileError: " << exp.what() << std::endl;
	}
	catch(const Glib::MarkupError& exp)
	{
		std::cerr << "MarkupError: " << exp.what() << std::endl;
	}
	catch(const Gtk::BuilderError& exp)
	{
		std::cerr << "BuilderError: " << exp.what() << std::endl;
	}
	
	Gtk::Dialog* mDialog = nullptr;
	Gtk::Button* mButton = nullptr;
	
	refBuilder->get_widget("DialogBasic", mDialog);
	if(mDialog)
	{
		refBuilder->get_widget("QuitButton", mButton);
		app->run(*mDialog);
	}
	
	ObjectLibrary obj1;
	obj1.SayHello_();
}

Library1::~Library1()
{
	
}

