
#include "functions/email_action.h"

using namespace CPW::Functions;

EmailAction::EmailAction(std::string identifier) :
    Action(identifier)
    ,app_(Application::instance())
    ,mail_host_("")
    ,sender_("")
    ,recipient_("")
    ,subject_("")
    ,email_message_("")
    ,email_user_("")
    ,email_password_("")
{
    set_action_type(ActionType::kEmail);
}

bool EmailAction::Work_()
{
    try
    {
        IdentifyParameters_();
        UpdateDataMembers_();
        UpdateMessageParameters_();

        if(mail_host_== "" || sender_== "" || recipient_== "" || subject_== "" || email_message_== "" || email_user_== "" || email_password_== "")
        {
            set_custom_error("Predefined parameters is empty.");
            return false;
        }

	    SSLInitializer sslInitializer;

        SharedPtr<Net::InvalidCertificateHandler> pCert = new Net::ConsoleCertificateHandler(false);
        Net::Context::Ptr pContext = new Net::Context(Net::Context::CLIENT_USE, "", "", "", Net::Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        Net::SSLManager::instance().initializeClient(0, pCert, pContext);

        Net::MailMessage message;
        message.setSender(sender_);
        message.addRecipient(Net::MailRecipient(Net::MailRecipient::PRIMARY_RECIPIENT, recipient_));
        message.setSubject(subject_);
        message.addContent(new Net::StringPartSource(email_message_));
        
        Net::SecureSMTPClientSession session(mail_host_);
        session.login();
        session.startTLS(pContext);
        if (!email_user_.empty())
        {
            session.login(Net::SMTPClientSession::AUTH_LOGIN, email_user_, email_password_);
        }
        session.sendMessage(message);
        session.close();

        return true;
    }
    catch(std::runtime_error& error)
    {
        std::string string_error = "- Error on query_actions.cc on ComposeQuery_(): " + std::string(error.what());
        app_.logger().error(string_error);
        set_error(true);
        set_custom_error(string_error);
        return false;
    }
    catch(std::exception& error)
    {
        std::string string_error = "- Error on query_actions.cc on ComposeQuery_(): " + std::string(error.what());
        app_.logger().error(string_error);
        set_error(true);
        set_custom_error(string_error);
        return false;
    }
    return false;
}

void EmailAction::UpdateDataMembers_()
{
    // Predefined parameters
    for(auto& param : get_parameters())
    {
        // Verify conditional parameter
        if(param.get_parameter_type() == Query::ParameterType::kConditional)
        {
            // Find action results
            auto action_found = std::find_if(get_actions().begin(), get_actions().end(),[&param](std::shared_ptr<Functions::Action>& action)
            {
                return action->get_identifier() == param.get_conditional_field_action();
            });

            if(action_found != get_actions().end())
            {
                auto row_value = action_found->get()->get_results()->FindField_(param.get_conditional_field());
                param.set_value(row_value->get_value());
            }
        }

        if(param.get_name() == "mailHost")
            mail_host_ = param.get_value().get_value_string();
        else if(param.get_name() == "sender")
            sender_ = param.get_value().get_value_string();
        else if(param.get_name() == "recipient")
            recipient_ = param.get_value().get_value_string();
        else if(param.get_name() == "subject")
            subject_ = param.get_value().get_value_string();
        else if(param.get_name() == "message")
            email_message_ = param.get_value().get_value_string();
        else if(param.get_name() == "emailUser")
            email_user_ = param.get_value().get_value_string();
        else if(param.get_name() == "emailPassword")
            email_password_ = param.get_value().get_value_string();
    }

}

void EmailAction::UpdateMessageParameters_()
{
    // Replace parameters in message
    for(auto& param : get_parameters())
    {
        if
        (
            param.get_name() == "mailHost" || param.get_name() == "sender" || param.get_name() == "recipient"
            || param.get_name() == "subject" || param.get_name() == "message" || param.get_name() == "emailUser"
            || param.get_name() == "emailPassword"
        )
            continue;

        // Verify conditional parameter
        if(param.get_parameter_type() == Query::ParameterType::kConditional)
        {
            // Find action results
            auto action_found = std::find_if(get_actions().begin(), get_actions().end(),[&param](std::shared_ptr<Functions::Action>& action)
            {
                return action->get_identifier() == param.get_conditional_field_action();
            });

            if(action_found != get_actions().end())
            {
                auto row_value = action_found->get()->get_results()->FindField_(param.get_conditional_field());
                param.set_value(row_value->get_value());
            }
        }

        std::string string_search = "${" + param.get_name() + "}";
        auto pos = email_message_.find(string_search);
        while (pos != std::string::npos)
        {
            email_message_.replace(pos, string_search.length(), param.get_value().get_value_string());
            pos = email_message_.find(string_search, pos + param.get_value().get_value_string().length());
        }
    }
}