// Qt
// Local
#include <TTSAddContract.h>
#include <Protocol.h>

using namespace hb::network;
using namespace pi;

TTSAddContract::TTSAddContract() :
    HbNetworkContract( Protocol::CHANNEL_TTS,
                       Protocol::CODE_SRV_TTS_ADD )
{
    setRouting( HbNetworkProtocol::ROUTING_MULTICAST );
}

TTSAddContract * TTSAddContract::create() const
{
    return new TTSAddContract();
}

QString TTSAddContract::toString() const
{
    return QString( "%1, size=%2" )
            .arg( HbNetworkContract::toString() )
            .arg( mTTSObjectsByUid.size() );
}

bool TTSAddContract::read( QDataStream & stream )
{
    stream >> mTTSObjectsByUid;

    return true;
}

bool TTSAddContract::write( QDataStream & stream ) const
{
    stream << mTTSObjectsByUid;

    return true;
}

const QHash< quint64, TTSObject > & TTSAddContract::ttsObjects() const
{
    return mTTSObjectsByUid;
}

void TTSAddContract::addTTSObject( const TTSObject & tts )
{
    if( !mTTSObjectsByUid.contains( tts.uid() ) )
    {
        mTTSObjectsByUid.insert( tts.uid(), tts );
    }
}
