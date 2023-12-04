
#ifndef FUNCTIONS_ACTION_H
#define FUNCTIONS_ACTION_H


#include <string>
#include <vector>

#include "tools/row_value_formatter.h"
#include "query/results.h"
#include "query/parameter.h"


namespace CPW
{
    namespace Functions
    {
        class Action;
    }
}


class CPW::Functions::Action
{
    public:
        Action(std::string identifier);

        std::string get_identifier() const { return identifier_; };
        std::string get_status() const { return status_; };
        std::string get_message() const { return message_; };
        std::string get_custom_error() const { return custom_error_; };
        bool get_final() const { return final_; };
        bool get_error() const { return error_; };
        std::vector<Query::Parameter>& get_parameters()
        {
            auto& var = parameters_;
            return var;
        }
        std::string get_sql_code() const { return sql_code_; };
        Query::Results& get_results()
        {
            auto& var = results_;
            return var;
        }

        void set_identifier(std::string identifier) { identifier_ = identifier; };
        void set_status(std::string status) { status_ = status; };
        void set_message(std::string message) { message_ = message; };
        void set_custom_error(std::string custom_error) { custom_error_ = custom_error; };
        void set_final(bool final) { final_ = final; };
        void set_error(bool error) { error_ = error; };
        void set_sql_code(std::string sql_code) { sql_code_ = sql_code; };

    private:
        std::string identifier_;
        std::string status_;
        std::string message_;
        std::string custom_error_;
        bool final_;
        bool error_;
        std::vector<Query::Parameter> parameters_;
        std::string sql_code_;
        Query::Results results_;
};

#endif // FUNCTIONS_ACTION_H
