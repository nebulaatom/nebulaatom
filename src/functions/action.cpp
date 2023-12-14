
#include "functions/action.h"
#include "query/results.h"
#include <memory>

using namespace CPW::Functions;

Action::Action(std::string identifier) :
    identifier_(identifier)
    ,status_("OK.")
    ,message_("OK.")
    ,custom_error_("")
    ,final_(false)
    ,error_(false)
    ,sql_code_("SELECT 1")
{
    results_ = std::make_shared<Query::Results>();
}
