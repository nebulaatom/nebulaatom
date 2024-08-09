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

#ifndef NAF_HANDLERS_BACKENDHANDLER
#define NAF_HANDLERS_BACKENDHANDLER


#include "handlers/root_handler.h"
#include "query/results.h"

namespace NAF
{
    namespace Handlers
    {
        class BackendHandler;
    }
}


class NAF::Handlers::BackendHandler : public RootHandler
{
    public:
        using VectorShrResults = std::vector<Query::Results::Ptr>;

        BackendHandler();

        Files::FileManager& get_file_manager()
        {
            auto& var = file_manager_;
            return var;
        }

    protected:
        virtual void Process_() override;
        void ProcessActions_();
        void ProcessJSONResponse_();
        void ProcessFileResponse_();

    private:
        Files::FileManager file_manager_;
};

#endif // NAF_HANDLERS_BACKENDHANDLER
