
#include "tools/hmac_tool.h"

Tools::HMACTool::HMACTool()
{

}

std::string Tools::HMACTool::Encode_(std::string text_to_encode)
{
	std::string passphrase(text_to_encode);
	Poco::HMACEngine<Poco::SHA1Engine> hmac(passphrase); // we'll compute a HMAC-SHA1
	hmac.update(text_to_encode);
	const Poco::DigestEngine::Digest& digest = hmac.digest();// finish HMAC computation and obtain digest
	std::string text_encoded(Poco::DigestEngine::digestToHex(digest));// convert to a string of hexadecimal numbers

    return text_encoded;
}
