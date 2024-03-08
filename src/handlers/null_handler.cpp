/*
* CPW Woodpecker Server
* Copyright (C) 2021 CPW Online support@cpwonline.org
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

#include "handlers/null_handler.h"

using namespace CPW::Handlers;

NullHandler::~NullHandler()
{

}

void NullHandler::AddFunctions_()
{

}

void NullHandler::Process_()
{
    SettingUpFunctions_();
}

void NullHandler::HandleGETMethod_()
{
    NullResponse_();
}

void NullHandler::HandlePOSTMethod_()
{
    NullResponse_();
}

void NullHandler::HandlePUTMethod_()
{
    NullResponse_();
}

void NullHandler::HandleDELMethod_()
{
    NullResponse_();
}

void NullHandler::NullResponse_()
{
    HTMLResponse_
    (
        *get_response()
        ,HTTPResponse::HTTP_NOT_FOUND
        ,"Sorry, content not found from" + get_request()->getMethod() + " request."
    );
}
