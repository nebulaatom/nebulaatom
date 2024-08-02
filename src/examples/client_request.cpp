
#include "http/client.h"
#include "http/methods.h"

using namespace NAF;

int main()
{
    HTTP::Client client("https://google.com", HTTP::HTTP_GET);
    client.SetupSSL_("key.pem", "cert.pem");
    client.AddHeader_("header-name", "header-value");
    client.AddCookie_("cookie-name", "cookie-value");
    client.set_response_handler([&](std::stringstream& response, Net::HTTPRequest&, Net::HTTPResponse&)
    {
        std::cout << response.str() << std::endl;
    });

    client.SendRequest_();
}
