// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ATOM_TOOLSANDLERCONNECTION
#define ATOM_TOOLSANDLERCONNECTION


#include <functional>

#include "handlers/root_handler.h"
#include "handlers/null_handler.h"
#include "handlers/backend_handler.h"
#include "handlers/frontend_handler.h"


namespace Atom
{
    namespace Tools
    {
        class HandlerConnection;
    }
}

class AtomTools::HandlerConnection
{
    public:
        using HandlerFunctor = std::function<AtomHandlers::RootHandler*()>;

        HandlerConnection(AtomTools::Route route, HandlerFunctor handler);
        ~HandlerConnection();

        AtomTools::Route current_route_;
        HandlerFunctor return_handler_;
};


#endif // ATOM_TOOLSANDLERCONNECTION
