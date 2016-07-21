#ifndef CLIENTPIUSER_H
#define CLIENTPIUSER_H

// Qt
#include <QtCore/QString>
#include <QtCore/QDataStream>
// Hb
#include <contract/HbNetworkProtocol.h>
#include <HbNetwork.h>
// Local
#include <PiUser.h>

namespace pi
{
    class ClientPiUser : public PiUser
    {
        Q_OBJECT
        Q_ENUMS( PiUserStatus )

        Q_PROPERTY( PiUserStatus status READ status NOTIFY statusChanged )

    public:
        enum PiUserStatus : netwint
        {
            PIUSER_DISCONNECTED = HbNetworkProtocol::USER_DISCONNECTED,
            PIUSER_CONNECTING = HbNetworkProtocol::USER_CONNECTING,
            PIUSER_CONNECTED = HbNetworkProtocol::USER_CONNECTED,
            PIUSER_AUTHENTICATING = HbNetworkProtocol::USER_AUTHENTICATING,
            PIUSER_AUTHENTICATED = HbNetworkProtocol::USER_AUTHENTICATED
        };

        ClientPiUser();
        ~ClientPiUser() = default;

        PiUserStatus status() const;
        void setStatus( PiUserStatus status );

    signals:
        void statusChanged();

    private:
        PiUserStatus mStatus;
    };
}

#endif // CLIENTPIUSER_H
