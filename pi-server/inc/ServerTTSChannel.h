#ifndef SERVERTTSCHANNEL_H
#define SERVERTTSCHANNEL_H

// Qt
// Hb
#include <service/channel/HbServerPeopledChannel.h>
// Local
#include <TTSObject.h>

namespace pi
{

    class ServerTTSChannel : public hb::network::HbServerPeopledChannel
    {
        Q_OBJECT
    public:

        ServerTTSChannel();
        virtual ~ServerTTSChannel() = default;

        virtual void reset() override;
        virtual void plugContracts( HbNetworkExchanges & exchanges ) override;
        virtual serviceuid uid() const override;

    public callbacks:
        // From network.
        virtual void onUserContractReceived( hb::network::ShConstHbNetworkUserInfo user_info, const HbNetworkContract * contract ) override;

        // From processor.
        void onTTSAdded( const TTSObject & tts ); // Broadcast.
        void onUserSynchonized( QList< TTSObject > tts, QString email ); // Newly connected user.
        void onTTSUpdated( quint64 tts_uid, TTSObject::TTSStatus status );

    signals:
        // To processor.
        void ttsReceived( const TTSObject & tts );
        void ttsRemoved( quint64 tts_uid, const QString & email );
        void userToSynchonize( const QString & email );
    };
}

#endif // SERVERTTSCHANNEL_H
