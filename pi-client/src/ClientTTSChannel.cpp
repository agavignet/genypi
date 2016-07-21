// Hb
#include <HbLogService.h>
// Local
#include <ClientTTSChannel.h>
#include <TTSSendContract.h>
#include <TTSAddContract.h>
#include <TTSRemoveContract.h>
#include <TTSUpdateContract.h>
#include <Protocol.h>

using namespace hb::network;
using namespace pi;

void ClientTTSChannel::reset()
{

}

void ClientTTSChannel::plugContracts( HbNetworkExchanges & exchanges )
{
    exchanges.plug< TTSSendContract >();
    exchanges.plug< TTSAddContract >();
    exchanges.plug< TTSUpdateContract >();
    exchanges.plug< TTSRemoveContract >();
}

serviceuid ClientTTSChannel::uid() const
{
    return Protocol::CHANNEL_TTS;
}

void ClientTTSChannel::sendTTS( const TTSObject & tts )
{
    TTSSendContract * tts_contract = new TTSSendContract();
    tts_contract->setTTSObject( tts );

    onContractToSend( tts_contract );
}

void ClientTTSChannel::removeTTS( quint64 tts_uid )
{
    TTSRemoveContract * tts_contract = new TTSRemoveContract();
    tts_contract->setTTSUid( tts_uid );

    onContractToSend( tts_contract );
}

void ClientTTSChannel::onUserContractReceived( const HbNetworkContract * contract )
{
    q_assert_ptr( contract );

    HbInfo( "Contract received from %d.", HbLatin1( contract->sender() ) );
    HbInfo( "Contract details: %s", HbLatin1( contract->toString() ) );

    const TTSUpdateContract * update_contract = contract->value< TTSUpdateContract >();
    const TTSAddContract * add_contract = contract->value< TTSAddContract >();
    if( update_contract )
    {
        emit ttsUpdated( update_contract->ttsUid(), update_contract->ttsStatus() );
    }
    else if( add_contract )
    {
        emit ttsAdded( add_contract->ttsObjects().values() );
    }

    delete contract;
}
