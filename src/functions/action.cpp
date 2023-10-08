
#include "functions/action.h"

using namespace CPW::Functions;

Action::Action(std::string identifier) :
    identifier_(identifier)
    ,status_("OK.")
    ,message_("OK.")
    ,custom_error_("")
    ,sql_code_("SELECT 1")
{

}
