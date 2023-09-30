
#ifndef CPW_FUNCTIONS_FUNCTIONS_MANAGER_H
#define CPW_FUNCTIONS_FUNCTIONS_MANAGER_H


#include <string>
#include <map>

#include "functions/function.h"


namespace CPW
{
    namespace Functions
    {
        class FunctionsManager;
    }
}


class CPW::Functions::FunctionsManager
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

#endif // CPW_FUNCTIONS_FUNCTIONS_MANAGER_H
