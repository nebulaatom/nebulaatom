
#ifndef FUNCTIONS_SQL_ACTION
#define FUNCTIONS_SQL_ACTION


#include <istream>
#include <string>
#include <array>
#include <map>
#include <stdexcept>
#include <vector>


#include <Poco/Net/HTTPServerRequest.h>
#include "Poco/Data/Session.h"
#include "Poco/Data/MySQL/Connector.h"
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/Statement.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/RecordSet.h>

#include "functions/action.h"
#include "tools/row_value_formatter.h"
#include "query/results.h"


namespace Atom
{
    namespace Functions
    {
        class SQLAction;
    }
}

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;


class DatabaseManager;
class SettingsManager;

class Atom::Functions::SQLAction :
    public Atom::Functions::Action
{
    public:
        SQLAction(std::string identifier);

        std::string get_sql_code() const { return sql_code_; };
        
        void set_sql_code(std::string sql_code) { sql_code_ = sql_code; };

        std::string get_final_query() const {return final_query_;}
        int get_affected_rows_() const {return affected_rows_;}
        std::shared_ptr<Data::Session>& get_session()
        {
            auto& var = session_;
            return var;
        }
        std::shared_ptr<Data::Statement>& get_query()
        {
            auto& var = query_;
            return var;
        }

        bool ComposeQuery_();
        void ExecuteQuery_();
        void MakeResults_();
        JSON::Object::Ptr CreateJSONResult_();

        virtual bool Work_() override;

    private:
        bool InitializeQuery_();

        std::string sql_code_;
        std::string final_query_;
        int affected_rows_;
        std::shared_ptr<Data::Session> session_;
        std::shared_ptr<Data::Statement> query_;
};

#endif // FUNCTIONS_SQL_ACTION
