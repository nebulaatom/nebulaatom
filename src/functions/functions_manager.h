
#ifndef ATOM_FUNCTIONS_FUNCTIONS_MANAGER
#define ATOM_FUNCTIONS_FUNCTIONS_MANAGER


#include <string>
#include <map>

#include "functions/function.h"
#include "http/methods.h"


namespace Atom
{
    namespace Functions
    {
        class FunctionsManager;
    }
}


class Atom::Functions::FunctionsManager
{
    public:
        using FunctionsMap = std::map<std::string, Function::Ptr>;

        FunctionsManager();

        FunctionsMap& get_functions()
        {
            auto& var = functions_;
            return var;
        }

        Function::Ptr AddFunction_(std::string endpoint, HTTP::EnumMethods method);

    private:
        FunctionsMap functions_;
};

#endif // ATOM_FUNCTIONS_FUNCTIONS_MANAGER
