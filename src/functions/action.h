
#ifndef FUNCTIONS_ACTION_H
#define FUNCTIONS_ACTION_H


#include <string>
#include <vector>
#include <memory>

#include "Poco/Util/Application.h"
#include "Poco/JSON/Object.h"

#include "query/results.h"
#include "query/parameter.h"
#include "query/condition.h"
#include "tools/manage_json.h"


namespace CPW
{
    namespace Functions
    {
        enum class ActionType;
        class Action;
    }
}

using namespace Poco;
using namespace Poco::Util;


enum class AtomFunctions::ActionType
{
    kSQL
    ,kEmail
};

class AtomFunctions::Action :
    public Tools::ManageJSON
{
    public:
        using ActionPtr = std::shared_ptr<Action>;

        Action(std::string identifier);
        virtual ~Action();

        ActionType get_action_type() const { return action_type_; }
        std::string get_identifier() const { return identifier_; };
        std::string get_status() const { return status_; };
        std::string get_message() const { return message_; };
        std::string get_custom_error() const { return custom_error_; };
        bool get_final() const { return final_; };
        bool get_error() const { return error_; };
        std::vector<Query::Condition>& get_conditions()
        {
            auto& var = conditions_;
            return var;
        }
        std::vector<Query::Parameter>& get_parameters()
        {
            auto& var = parameters_;
            return var;
        }
        std::shared_ptr<Query::Results>& get_results()
        {
            auto& var = results_;
            return var;
        }
        JSON::Object::Ptr& get_json_result()
        {
            auto& var = json_result_;
            return var;
        }
        std::vector<ActionPtr>& get_actions()
        {
            auto& var = actions_;
            return var;
        }

        void set_action_type(ActionType action_type) { action_type_ = action_type; }
        void set_identifier(std::string identifier) { identifier_ = identifier; };
        void set_status(std::string status) { status_ = status; };
        void set_message(std::string message) { message_ = message; };
        void set_custom_error(std::string custom_error) { custom_error_ = custom_error; };
        void set_final(bool final) { final_ = final; };
        void set_error(bool error) { error_ = error; };

        void IdentifyParameters_();
        virtual bool Work_() = 0;

    private:
        Application& app_;
        ActionType action_type_;
        std::string identifier_;
        std::string status_;
        std::string message_;
        std::string custom_error_;
        bool final_;
        bool error_;
        std::vector<Query::Condition> conditions_;
        std::vector<Query::Parameter> parameters_;
        std::shared_ptr<Query::Results> results_;
        JSON::Object::Ptr json_result_;
        std::vector<ActionPtr> actions_;
};

#endif // FUNCTIONS_ACTION_H
