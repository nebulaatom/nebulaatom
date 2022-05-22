// <one line to give the program's name and a brief idea of what it does.>
// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CPW_TOOLS_HANDLERCONNECTION_H
#define CPW_TOOLS_HANDLERCONNECTION_H


#include <functional>

#include "handlers/root_handler.h"
#include "handlers/null_handler.h"
#include "handlers/backend_handler.h"
#include "handlers/frontend_handler.h"


namespace CPW
{
    namespace Tools
    {
        class HandlerConnection;
    }
}

class CPW::Tools::HandlerConnection
{
    public:
        using HandlerFunctor = std::function<CPW::Handlers::RootHandler*()>;

        HandlerConnection(CPW::Tools::Route route, HandlerFunctor handler);
        ~HandlerConnection();

        CPW::Tools::Route current_route_;
        HandlerFunctor return_handler_;
};


#endif // CPW_TOOLS_HANDLERCONNECTION_H
