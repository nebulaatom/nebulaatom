
#include "core/nebula_atom.h"
#include "files/file_manager.h"
#include "handlers/frontend_handler.h"
#include "http/request.h"

using namespace NAF;

class Webserver : public Handlers::FrontendHandler
{
    public:
        Webserver(){}
        virtual ~Webserver(){}

        void Process_()
        {
            get_file_manager().set_directory_base("/var/www");
            get_file_manager().AddBasicSupportedFiles_();
            DownloadProcess_();
        }
};

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    // Setting up handler
        app.CustomHandlerCreator_([&](Core::HTTPRequestInfo&)
        {
            return new Webserver;
        });

        return app.Init_(argc, argv);

}
