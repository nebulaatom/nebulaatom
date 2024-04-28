
#ifndef FUNCTIONS_ACTION
#define FUNCTIONS_ACTION


#include <string>
#include <vector>
#include <memory>

#include "Poco/JSON/Object.h"

#include "query/results.h"
#include "query/parameter.h"
#include "query/condition.h"
#include "tools/manage_json.h"
#include "tools/output_logger.h"


namespace Atom
{
    namespace Functions
    {
        enum class ActionType;
        class Action;
    }
}

using namespace Poco;


enum class Atom::Functions::ActionType
{
    kSQL
    ,kEmail
};

class Atom::Functions::Action :
    public Tools::ManageJSON
{
    public:
        using Ptr = std::shared_ptr<Action>;

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
        std::vector<Ptr>& get_actions()
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
        Query::Parameter& AddParameter_(std::string name, Tools::RowValueFormatter value, bool editable);
        Query::Parameter& AddParameter_(std::string name, Query::ConditionalField conditional_field, std::string conditional_field_action, bool editable);
        Query::Condition& AddCondition_(Query::ConditionType type, Tools::RowValueFormatter row_value, Query::ConditionalField conditional_field);
        Query::Condition& AddCondition_(std::vector<Tools::RowValueFormatter> row_values, Query::ConditionalField conditional_field);
        virtual bool Work_() = 0;

    private:
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
        std::vector<Ptr> actions_;
};

#endif // FUNCTIONS_ACTION
