// Qt
// Local
#include <TTSUpdateContract.h>
#include <Protocol.h>

using namespace hb::network;
using namespace pi;

TTSUpdateContract::TTSUpdateContract() :
    HbNetworkContract( Protocol::CHANNEL_TTS,
                       Protocol::CODE_SRV_TTS_UPDATE )
{
    setRouting( HbNetworkProtocol::ROUTING_MULTICAST );
}

TTSUpdateContract * TTSUpdateContract::create() const
{
    return new TTSUpdateContract();
}

QString TTSUpdateContract::toString() const
{
    return QString( "%1, ttsUid=%2, ttsStatus=%3" )
            .arg( HbNetworkContract::toString() )
            .arg( mTTSUid )
            .arg( TTSObject::MetaTTSStatus::toString( mTTSStatus ) );
}

bool TTSUpdateContract::read( QDataStream & stream )
{
    quint8 status;
    stream >> mTTSUid;
    stream >> status;

    mTTSStatus = ( TTSObject::TTSStatus ) status;

    return true;
}

bool TTSUpdateContract::write( QDataStream & stream ) const
{
    stream << mTTSUid;
    stream << ( quint8 ) mTTSStatus;

    return true;
}

quint64 TTSUpdateContract::ttsUid() const
{
    return mTTSUid;
}

void TTSUpdateContract::setTTSUid( quint64 tts_uid )
{
    mTTSUid = tts_uid;
}

TTSObject::TTSStatus TTSUpdateContract::ttsStatus() const
{
    return mTTSStatus;
}

void TTSUpdateContract::setTTSStatus( TTSObject::TTSStatus tts_status )
{
    mTTSStatus = tts_status;
}
