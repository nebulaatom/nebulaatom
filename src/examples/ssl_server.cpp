
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "tools/settings_manager.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app(true);
    Tools::SettingsManager::ReadBasicProperties_();

    app.CustomHandlerCreator_([&](const HTTPServerRequest&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.SetupSSL_();
            self.CustomHTMLResponse_(HTTP::Status::kOK, "SSL Hello.");
        });
    });

    return app.run(argc, argv);
}
