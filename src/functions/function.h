
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
        using ActionPtr = std::shared_ptr<Action>;

        Function();
        Function(std::string endpoint, HTTP::EnumMethods type);

        std::string get_endpoint() const { return endpoint_; }
        std::string get_endpoint2() const { return endpoint2_; }
        std::string get_target() const { return target_; }
        HTTP::EnumMethods get_type() const { return type_; }
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
        void set_endpoint2(std::string endpoint2) { endpoint2_ = endpoint2; }
        void set_target(std::string target) { target_ = target; }
        void set_type(HTTP::EnumMethods type) { type_ = type; }

    private:
        std::string endpoint_;
        std::string endpoint2_;
        std::string target_;
        HTTP::EnumMethods type_;
        std::vector<ActionPtr> actions_;
        HTTP::Methods methods_;
};


#endif // ATOM_FUNCTIONS_FUNCTION
