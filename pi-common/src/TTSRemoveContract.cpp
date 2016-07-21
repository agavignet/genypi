// Qt
// Local
#include <TTSRemoveContract.h>
#include <Protocol.h>

using namespace hb::network;
using namespace pi;

TTSRemoveContract::TTSRemoveContract() :
    HbNetworkContract( Protocol::CHANNEL_TTS,
                       Protocol::CODE_CLT_TTS_REMOVE )
{
    setRouting( HbNetworkProtocol::ROUTING_UNICAST );
}

TTSRemoveContract * TTSRemoveContract::create() const
{
    return new TTSRemoveContract();
}

QString TTSRemoveContract::toString() const
{
    return QString( "%1, ttsUid=%2" )
            .arg( HbNetworkContract::toString() )
            .arg( mTTSUid );
}

bool TTSRemoveContract::read( QDataStream & stream )
{
    stream >> mTTSUid;

    return true;
}

bool TTSRemoveContract::write( QDataStream & stream ) const
{
    stream << mTTSUid;

    return true;
}

quint64 TTSRemoveContract::ttsUid() const
{
    return mTTSUid;
}

void TTSRemoveContract::setTTSUid( quint64 tts_uid )
{
    mTTSUid = tts_uid;
}
