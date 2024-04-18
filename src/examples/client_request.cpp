
#include "http/client.h"

using namespace Atom;

int main()
{
    HTTP::Client client("https://localhost:8080");
    client.set_response_handler([&](std::stringstream& response, Net::HTTPRequest&, Net::HTTPResponse&)
    {
        std::cout << response.str() << std::endl;
    });

    client.SendRequest_();
}
