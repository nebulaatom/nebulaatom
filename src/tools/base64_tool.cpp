
#include "tools/base64_tool.h"

Tools::Base64Tool::Base64Tool()
{

}

std::string Tools::Base64Tool::Encode_(std::string text_to_encode)
{
    std::stringstream str_enc("");

    Poco::Base64Encoder encoder(str_enc);
    encoder << text_to_encode;
    encoder.close();

    return str_enc.str();
}

std::string Tools::Base64Tool::Decode_(std::string text_to_decode)
{
    std::stringstream str_dec(text_to_decode);
    Poco::Base64Decoder decoder(str_dec);
    std::stringstream out;
    Poco::StreamCopier::copyStream(decoder, out);
    return out.str();
}