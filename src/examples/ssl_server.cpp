
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "tools/settings_manager.h"

using namespace NAF;

int main(int argc, char** argv)
{
    Core::NebulaAtom app(true);
    Tools::SettingsManager::set_config_path("properties.yaml");
    Tools::SettingsManager::ReadSettings_();
    app.SetupSettings_();

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.SetupSSL_();
            self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, "SSL Hello.");
        });
    });

    return app.Init_(argc, argv);
}
