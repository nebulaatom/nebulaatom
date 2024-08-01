
#include "tools/email.h"
#include <Poco/Net/NetSSL.h>
#include <stdexcept>

using namespace Atom::Tools;

Email::Email() :
    mail_host_("")
    ,sender_("")
    ,recipient_("")
    ,subject_("")
    ,email_message_("")
    ,email_user_("")
    ,email_password_("")
{
    
}

bool Email::SendEmail_()
{
    try
    {
        Net::initializeSSL();

        if(mail_host_== "" || sender_== "" || recipient_== "" || subject_== "" || email_message_== "" || email_user_== "" || email_password_== "")
        {
            throw std::runtime_error("Predefined parameters is empty.");
            return false;
        }

	    Net::initializeSSL();

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
        std::string string_error = "Error on email.cpp on SendEmail_(): " + std::string(error.what());
        Tools::OutputLogger::Error_(string_error);
        return false;
    }
    catch(std::exception& error)
    {
        std::string string_error = "Error on email.cpp on SendEmail_(): " + std::string(error.what());
        Tools::OutputLogger::Error_(string_error);
        return false;
    }
    return false;
}
