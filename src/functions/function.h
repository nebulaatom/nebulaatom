
#ifndef NAF_FUNCTIONS_FUNCTION
#define NAF_FUNCTIONS_FUNCTION


#include <string>
#include <vector>
#include <map>
#include <memory>

#include "functions/action.h"
#include "http/methods.h"
#include "query/parameter.h"


namespace NAF
{
    namespace Functions
    {
        class Function;
    }
}


class NAF::Functions::Function
{
    public:
        using Ptr = std::shared_ptr<Functions::Function>;

        enum class ResponseType
        {
            kJSON
            ,kFile
        };

        Function();
        Function(std::string endpoint, HTTP::EnumMethods type);

        std::string get_endpoint() const { return endpoint_; }
        std::string get_target() const { return target_; }
        bool get_error() const { return error_; }
        std::string get_error_message() const { return error_message_; }
        ResponseType get_response_type() const { return response_type_; }
        HTTP::EnumMethods get_method() const { return method_; }
        std::vector<Action::Ptr>& get_actions()
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
        void set_response_type(ResponseType response_type) { response_type_ = response_type; }

        Action::Ptr AddAction_(std::string identifier);
        bool ProcessJSONResponse_(JSON::Object::Ptr& json_result);

    private:
        std::string endpoint_;
        std::string target_;
        bool error_;
        std::string error_message_;
        ResponseType response_type_;
        HTTP::EnumMethods method_;
        std::vector<Action::Ptr> actions_;
        HTTP::Methods methods_;
};


#endif // NAF_FUNCTIONS_FUNCTION
