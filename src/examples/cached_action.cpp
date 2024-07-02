
#include "core/nebula_atom.h"
#include "functions/cache_action.h"
#include "handlers/custom_handler.h"

using namespace Atom;

int main(int argc, char** argv)
{
    Core::NebulaAtom app;

    Query::DatabaseManager::StartMySQL_();
    Tools::SettingsManager::ReadBasicProperties_();

    // Cached Action
        Functions::CacheAction a1("a1", 5s);
        a1.set_sql_code("SELECT * FROM products");
        a1.set_final(true);
        a1.Work_();

    // Custom handler creator
        app.CustomHandlerCreator_([&](Core::HTTPRequestInfo&)
        {
            return new Handlers::CustomHandler([&](Handlers::CustomHandler& self)
            {
                // Process the request body
                self.ManageRequestBody_();

                std::mutex mutex;
                mutex.lock();
                self.CompoundResponse_(HTTP::Status::kHTTP_OK, a1.get_json_result());
                mutex.unlock();
            });
        });

    // Run
        auto code = app.Init_(argc, argv);

    // End
        a1.set_state(false);
        a1.get_updates_thread()->join();
        Query::DatabaseManager::StopMySQL_();
        return code;
}
