
#include "core/nebula_atom.h"
#include "handlers/custom_handler.h"
#include "tools/email.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app(true);
    Tools::SettingsManager::ReadBasicProperties_();

    app.CustomHandlerCreator_([&](Core::HTTPRequestInfo&)
    {
        return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
        {
            self.SetupSSL_();

            Tools::Email a("a1");
            a.set_mail_host("smtp.host.com");
            a.set_subject("Test Email");
            a.set_email_user("user@host.com");
            a.set_sender("user@host.com");
            a.set_recipient("recipient@host.com");
            a.set_email_password("password");
            a.set_email_message("Hello!");
            a.SendEmail_();
            self.CustomHTMLResponse_(HTTP::Status::kHTTP_OK, "Email sended.");
        });
    });

    return app.Init_(argc, argv);
}
