
#ifndef ATOMFUNCTIONS_FUNCTIONS_MANAGER_H
#define ATOMFUNCTIONS_FUNCTIONS_MANAGER_H


#include <string>
#include <map>

#include "functions/function.h"


namespace Atom
{
    namespace Functions
    {
        class FunctionsManager;
    }
}


class AtomFunctions::FunctionsManager
{
    public:
        FunctionsManager();

        std::map<std::string, Functions::Function>& get_functions()
        {
            auto& var = functions_;
            return var;
        }

    private:
        std::map<std::string, Functions::Function> functions_;
};

#endif // ATOMFUNCTIONS_FUNCTIONS_MANAGER_H
