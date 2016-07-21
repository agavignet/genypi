// Hb
#include <HbLogService.h>
// Local
#include <ServerTTSChannel.h>
#include <Protocol.h>
#include <TTSAddContract.h>
#include <TTSRemoveContract.h>
#include <TTSSendContract.h>
#include <TTSUpdateContract.h>

using namespace hb::network;
using namespace pi;

ServerTTSChannel::ServerTTSChannel() : HbServerPeopledChannel()
{
    connect( this, &HbServerPeopledChannel::userConnected, [this]( ShConstHbNetworkUserInfo user_info ) {
        emit userToSynchonize( user_info->email() ); // To processor.
    });
}

void ServerTTSChannel::reset()
{

}

void ServerTTSChannel::plugContracts( HbNetworkExchanges & exchanges )
{
    exchanges.plug< TTSSendContract >();
    exchanges.plug< TTSAddContract >();
    exchanges.plug< TTSUpdateContract >();
    exchanges.plug< TTSRemoveContract >();
}

serviceuid ServerTTSChannel::uid() const
{
    return Protocol::CHANNEL_TTS;
}

void ServerTTSChannel::onUserContractReceived( ShConstHbNetworkUserInfo user_info, const HbNetworkContract * contract )
{
    q_assert_ptr( contract );

    HbInfo( "Contract received from %d.", HbLatin1( user_info->email() ) );
    HbInfo( "Contract details: %s", HbLatin1( contract->toString() ) );

    const TTSSendContract   * send_contract   = contract->value< TTSSendContract >();
    const TTSRemoveContract * remove_contract = contract->value< TTSRemoveContract >();

    if( send_contract )
    {
        TTSObject tts = send_contract->ttsObject();
        tts.owner().setEmail( user_info->email() );
        tts.owner().setNickname( user_info->nickname() );
        tts.owner().setPicture( user_info->picture() );
        emit ttsReceived( tts );
    }
    else if( remove_contract )
    {
        emit ttsRemoved( remove_contract->ttsUid(), user_info->email() );
    }

    delete contract;
}

void ServerTTSChannel::onTTSAdded( const TTSObject & tts )
{
    TTSAddContract * add_contract = new TTSAddContract();
    add_contract->addTTSObject( tts );

    onUsersContractToSend( connectedUsers().values(), add_contract );
}

void ServerTTSChannel::onUserSynchonized( QList< TTSObject > tts, QString email )
{
    TTSAddContract * add_tts = new TTSAddContract();

    for( TTSObject tts_object : tts )
    {
        add_tts->addTTSObject( tts_object );
    }

    onUserContractToSend( connectedUser( email ), add_tts );
}

void ServerTTSChannel::onTTSUpdated( quint64 tts_uid, TTSObject::TTSStatus status )
{
    TTSUpdateContract * update_tts = new TTSUpdateContract();
    update_tts->setTTSUid( tts_uid );
    update_tts->setTTSStatus( status );

    onUsersContractToSend( connectedUsers().values(), update_tts );
}

