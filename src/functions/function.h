
#ifndef CPW_FUNCTIONS_FUNCTION_H
#define CPW_FUNCTIONS_FUNCTION_H


#include <string>
#include <vector>
#include <map>
#include <memory>

#include "functions/action.h"
#include "query/parameter.h"


namespace CPW
{
    namespace Functions
    {
        class Function;
    }
}


class CPW::Functions::Function
{
    public:
        enum class Type
        {
            kPOST
            ,kGET
            ,kPUT
            ,kDEL
        };

        Function(std::string endpoint, Type type);

        std::string get_endpoint() const { return endpoint_; }
        Type get_type() const { return type_; }
        std::vector<Functions::Action>& get_actions()
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
        void set_type(Type type) { type_ = type; }

    private:
        std::string endpoint_;
        Type type_;
        std::vector<Functions::Action> actions_;
        std::map<std::string, Type> methods_;
};


#endif // CPW_FUNCTIONS_FUNCTION_H
