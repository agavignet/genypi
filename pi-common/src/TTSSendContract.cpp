// Qt
// Local
#include <TTSSendContract.h>
#include <Protocol.h>

using namespace hb::network;
using namespace pi;

TTSSendContract::TTSSendContract() :
    HbNetworkContract( Protocol::CHANNEL_TTS,
                       Protocol::CODE_CLT_TTS_SEND )
{
    setRouting( HbNetworkProtocol::ROUTING_UNICAST );
}

TTSSendContract * TTSSendContract::create() const
{
    return new TTSSendContract();
}

QString TTSSendContract::toString() const
{
    return QString( "%1, %2" )
            .arg( HbNetworkContract::toString() )
            .arg( mTTSObject.toString() );
}

bool TTSSendContract::read( QDataStream & stream )
{
    stream >> mTTSObject;

    return true;
}

bool TTSSendContract::write( QDataStream & stream ) const
{
    stream << mTTSObject;

    return true;
}

const TTSObject & TTSSendContract::ttsObject() const
{
    return mTTSObject;
}

void TTSSendContract::setTTSObject( const TTSObject & tts )
{
    mTTSObject = tts;
}
