
#include "query/condition.h"

using namespace Atom::Query;

Condition::Condition(std::string identifier, ConditionType type, Functor functor) :
    identifier_(identifier)
    ,type_(type)
    ,functor_(functor)
{

}

bool Condition::VerifyCondition_(Results::Ptr results)
{
    return functor_(results);
}
