// Qt
#include <QTimer>
// Hb
// Local
#include <ClientPiUser.h>

using namespace pi;
using namespace hb::network;

ClientPiUser::ClientPiUser() : PiUser()
{
    mStatus = ClientPiUser::PIUSER_DISCONNECTED;
}

ClientPiUser::PiUserStatus ClientPiUser::status() const
{
    return mStatus;
}

void ClientPiUser::setStatus( PiUserStatus status )
{
    if( mStatus != status )
    {
        mStatus = status;
        emit statusChanged();
    }
}

