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

#ifndef ATOM_HANDLERS_BACKENDHANDLER
#define ATOM_HANDLERS_BACKENDHANDLER


#include "handlers/root_handler.h"
#include "query/results.h"

namespace Atom
{
    namespace Handlers
    {
        class BackendHandler;
    }
}


class Atom::Handlers::BackendHandler : public RootHandler
{
    public:
        using VectorShrResults = std::vector<std::shared_ptr<Query::Results>>;

        using RootHandler::RootHandler;
        virtual ~BackendHandler();

    protected:
        virtual void Process_() override;

        void ProcessActions_();

    private:
        VectorShrResults shared_results_;
};

#endif // ATOM_HANDLERS_BACKENDHANDLER
