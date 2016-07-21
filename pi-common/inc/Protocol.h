#ifndef PROTOCOL_H
#define PROTOCOL_H

// Qt
// Hb
#include <contract/HbNetworkProtocol.h>
// Local


namespace pi
{
    class Protocol
    {
    public:
        enum Channel : serviceuid
        {
            CHANNEL_TTS = hb::network::HbNetworkProtocol::SERVICE_USER + 1
        };

        enum Code : codeuid
        {
            CODE_CLT_TTS_SEND = hb::network::HbNetworkProtocol::CODE_USER + 1,
            CODE_SRV_TTS_ADD,
            CODE_SRV_TTS_UPDATE,
            CODE_CLT_TTS_REMOVE
        };
    };
}

#endif // PROTOCOL_H
