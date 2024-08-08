// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NAF_TOOLS_HANDLERCONNECTION
#define NAF_TOOLS_HANDLERCONNECTION


#include <functional>

#include "handlers/root_handler.h"
#include "handlers/null_handler.h"
#include "handlers/backend_handler.h"


namespace NAF
{
    namespace Tools
    {
        class HandlerConnection;
    }
}

class NAF::Tools::HandlerConnection
{
    public:
        using HandlerFunctor = std::function<NAF::Handlers::RootHandler*()>;

        HandlerConnection(NAF::Tools::Route route, HandlerFunctor handler);
        ~HandlerConnection();

        NAF::Tools::Route current_route_;
        HandlerFunctor return_handler_;
};


#endif // NAF_TOOLS_HANDLERCONNECTION
