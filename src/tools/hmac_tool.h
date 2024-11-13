
#ifndef NAF_TOOLS_HMACTOOL
#define NAF_TOOLS_HMACTOOL

#include <string>

#include "Poco/HMACEngine.h"
#include "Poco/SHA1Engine.h"


namespace NAF
{
    namespace Tools
    {
        class HMACTool;
    }
}

using namespace NAF;

class NAF::Tools::HMACTool
{
    public:
        HMACTool();

        std::string Encode_(std::string text_to_encode);
};

#endif //NAF_TOOLS_HMACTOOL