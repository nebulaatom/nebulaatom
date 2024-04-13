
#include "core/nebula_atom.h"
#include "files/file_manager.h"
#include "handlers/frontend_handler.h"
#include "http/request.h"

using namespace Atom;

class UploadFile : public Handlers::FrontendHandler
{
    public:
        UploadFile(){}
        virtual ~UploadFile(){}

        void Process_()
        {
            get_file_manager().set_directory_base("/var/www");
            get_file_manager().AddBasicSupportedFiles_();
            HandleGETMethod_();
        }
};

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    // Setting up handler
        app.CustomHandlerCreator_([&](HTTP::Request&)
        {
            return new UploadFile;
        });

        return app.Init_(argc, argv);

}
