/****************************************************************************
** Copyright (C) 2012-2015 Adrien Gavignet (hasboeuf)
** Contact: adrien.gavignet@gmail.com
**
** This file is part of the Hb module. Provided "AS IS", WITHOUT WARRANTIES
** OR CONDITIONS OF ANY KIND, either express or implied.
****************************************************************************/

#ifndef CLIENTTTSCHANNEL_H
#define CLIENTTTSCHANNEL_H

// Qt
// Hb
#include <service/channel/HbClientPeopledChannel.h>
// Local
#include <TTSObject.h>

namespace pi
{
    class ClientTTSChannel : public hb::network::HbClientPeopledChannel
    {
        Q_OBJECT

    public:

        ClientTTSChannel() = default;
        virtual ~ClientTTSChannel() = default;

        virtual void reset() override;
        virtual void plugContracts( HbNetworkExchanges & exchanges ) override;
        virtual serviceuid uid() const override;

        void sendTTS( const TTSObject & tts );
        void removeTTS( quint64 tts_uid );

    public callbacks:
        // From network.
        virtual void onUserContractReceived( const HbNetworkContract * contract ) override;

    signals:
        // To Client.
        void ttsAdded( QList< TTSObject > tts_objects );
        void ttsUpdated( quint64 tts_uid, TTSObject::TTSStatus tts_status );

    };
}

#endif // CLIENTTTSCHANNEL_H
