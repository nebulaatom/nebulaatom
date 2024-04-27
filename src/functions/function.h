
#ifndef ATOM_FUNCTIONS_FUNCTION
#define ATOM_FUNCTIONS_FUNCTION


#include <string>
#include <vector>
#include <map>
#include <memory>

#include "functions/action.h"
#include "http/methods.h"
#include "query/parameter.h"


namespace Atom
{
    namespace Functions
    {
        class Function;
    }
}


class Atom::Functions::Function
{
    public:
        using Ptr = std::shared_ptr<Functions::Function>;
        using ActionPtr = std::shared_ptr<Action>;

        Function();
        Function(std::string endpoint, HTTP::EnumMethods type);

        std::string get_endpoint() const { return endpoint_; }
        std::string get_target() const { return target_; }
        HTTP::EnumMethods get_method() const { return method_; }
        std::vector<ActionPtr>& get_actions()
        {
            auto& var = actions_;
            return var;
        }
        HTTP::Methods& get_methods()
        {
            auto& var = methods_;
            return var;
        }

        void set_endpoint(std::string endpoint) { endpoint_ = endpoint; }
        void set_target(std::string target) { target_ = target; }
        void set_method(HTTP::EnumMethods type) { method_ = type; }

    private:
        std::string endpoint_;
        std::string target_;
        HTTP::EnumMethods method_;
        std::vector<ActionPtr> actions_;
        HTTP::Methods methods_;
};


#endif // ATOM_FUNCTIONS_FUNCTION
