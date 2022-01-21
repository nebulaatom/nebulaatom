// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tools/handler_connection.h"

using namespace CPW::Tools;

HandlerConnection::HandlerConnection(Tools::Route route, HandlerFunctor handler) :
	current_route_(route)
	,return_handler_(handler)
{

}

HandlerConnection::~HandlerConnection()
{

}
