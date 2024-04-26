/*
* Nebula Atom

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

using namespace Atom::Handlers;

NullHandler::~NullHandler()
{

}

void NullHandler::AddFunctions_()
{

}

void NullHandler::Process_()
{
    HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Sorry, content not found from " + get_http_server_request().value()->getMethod() + " request.");
}
