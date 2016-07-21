// Qt
// Hb
#include <HbLogService.h>
// Local
#include <ClientModel.h>

using namespace hb::network;
using namespace pi;

ClientModel::ClientModel( QObject * parent ) :
    QObject( parent )
{
    mBrowserNeeded = false;

    connect( &mMe, &ClientPiUser::statusChanged, this, &ClientModel::onMeStatusChanged );
}

ClientPiUser * ClientModel::me()
{
    return &mMe;
}

TTSObject * ClientModel::pendingTTS()
{
    mPendingTTS.reset();
    return &mPendingTTS;
}

ClientTTSModel * ClientModel::ttsModel()
{
    return &mTTSModel;
}



ClientPiConfigModel * ClientModel::piConfigModel()
{
    return &mPiConfigModel;
}

QUrl ClientModel::currentUrl() const
{
    return mCurrentUrl;
}

void ClientModel::setCurrentUrl( const QUrl & url )
{
    if( mCurrentUrl != url )
    {
        mCurrentUrl = url;
        emit currentUrlChanged();
    }
}

bool ClientModel::isBrowserNeeded() const
{
    return mBrowserNeeded;
}

void ClientModel::setBrowserNeeded( bool needed )
{
    if( mBrowserNeeded != needed )
    {
        mBrowserNeeded = needed;
        emit browserNeededChanged();
    }
}

void ClientModel::sendTTS()
{
    HbInfo( "send TTS: %s", HbLatin1( mPendingTTS.toString() ) );
    emit ttsSent( mPendingTTS );
    emit newNotification( "TTS Sent" );
}

void ClientModel::removeTTS( quint64 tts_uid )
{
    HbInfo( "Remove TTS: %d", tts_uid );
    emit ttsRemoved( tts_uid );
    emit newNotification( "TTS Removed" );
}

void ClientModel::onJoinRequested( const QString & config_label )
{
    ClientPiConfig * config = mPiConfigModel.piConfig( config_label );
    if( !config )
    {
        HbError( "No config found." );
        return;
    }

    emit joinRequested( ClientPiConfig( *config ) );
}

void ClientModel::onLoginRequested()
{
    emit loginRequested();
}

void ClientModel::onLeaveRequested()
{
    emit leaveRequested();
}

void ClientModel::onMeStatusChanged()
{
    static auto previous = mMe.status();

    ClientPiUser::PiUserStatus current = mMe.status();

    if( previous == ClientPiUser::PIUSER_AUTHENTICATED &&
        current != previous )
    {
        mTTSModel.reset();
    }

    previous = current;
}
