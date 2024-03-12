
#ifndef ATOMFUNCTIONS_FUNCTION_H
#define ATOMFUNCTIONS_FUNCTION_H


#include <string>
#include <vector>
#include <map>
#include <memory>

#include "functions/action.h"
#include "query/parameter.h"


namespace Atom
{
    namespace Functions
    {
        class Function;
    }
}


class AtomFunctions::Function
{
    public:
        using ActionPtr = std::shared_ptr<Action>;

        enum class Type
        {
            kPOST
            ,kGET
            ,kPUT
            ,kDEL
        };

        Function();
        Function(std::string endpoint, Type type);

        std::string get_endpoint() const { return endpoint_; }
        std::string get_endpoint2() const { return endpoint2_; }
        std::string get_target() const { return target_; }
        Type get_type() const { return type_; }
        std::vector<ActionPtr>& get_actions()
        {
            auto& var = actions_;
            return var;
        }
        std::map<std::string, Type>& get_methods()
        {
            auto& var = methods_;
            return var;
        }

        void set_endpoint(std::string endpoint) { endpoint_ = endpoint; }
        void set_endpoint2(std::string endpoint2) { endpoint2_ = endpoint2; }
        void set_target(std::string target) { target_ = target; }
        void set_type(Type type) { type_ = type; }

    private:
        std::string endpoint_;
        std::string endpoint2_;
        std::string target_;
        Type type_;
        std::vector<ActionPtr> actions_;
        std::map<std::string, Type> methods_;
};


#endif // ATOMFUNCTIONS_FUNCTION_H
