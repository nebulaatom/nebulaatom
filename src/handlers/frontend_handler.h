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

#ifndef ATOM_HANDLERS_FRONTENDHANDLER
#define ATOM_HANDLERS_FRONTENDHANDLER

#include <string>
#include <map>
#include <vector>

#include "handlers/root_handler.h"
#include "files/file_manager.h"


namespace Atom
{
    namespace Handlers
    {
        class FrontendHandler;
    }
}


class Atom::Handlers::FrontendHandler :
    public RootHandler
{
    public:
        FrontendHandler();
        ~FrontendHandler();

        Files::FileManager& get_file_manager()
        {
            auto& var = file_manager_;
            return var;
        }

    protected:
        virtual void AddFunctions_() override;
        virtual void Process_() override;
        virtual void HandleGETMethod_() override;
        virtual void HandlePOSTMethod_() override;
        virtual void HandlePUTMethod_() override;
        virtual void HandleDELMethod_() override;

    private:
        Files::FileManager file_manager_;
};

#endif // ATOM_HANDLERS_FRONTENDHANDLER
