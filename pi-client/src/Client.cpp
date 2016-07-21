// Qt
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
// Hb
#include <HbGlobal.h>
#include <HbLogService.h>
#include <gui/HbLogWidget.h>
#include <HbLoggerOutputs.h>
#include <HbClient.h>
#include <contract/auth/HbOAuthRequestContract.h>
#include <google/HbO2ClientGoogle.h>
// Local
#include <Client.h>
#include <Protocol.h>
#include <TTSAddContract.h>
#include <ClientTTSChannel.h>

using namespace hb::tools;
using namespace hb::log;
using namespace hb::network;
using namespace pi;

QString Client::msClientId;

Client::Client( QQmlApplicationEngine * qml_engine ) :
    QObject( qml_engine )
{
    qRegisterMetaType< TTSObject >( "TTSObject" );
    qRegisterMetaType< QList< TTSObject > >( "QList< TTSObject >" );
    qRegisterMetaType< TTSObject::TTSStatus >( "TTSObject::TTSStatus" );
    qRegisterMetaType< ClientPiConfig >( "ClientPiParameter" );
    qRegisterMetaType< QList< ClientPiConfig > >( "QList< ClientPiParameter >" );
    qRegisterMetaType< QList< ClientPiConfig > * >( "QList< ClientPiParameter > * " );

    connect( &mBrowserControls, &ClientBrowserControls::openBrowser, this, &Client::onOpenBrowser );
    connect( &mBrowserControls, &ClientBrowserControls::closeBrowser, this, &Client::onCloseBrowser );

    mTcpClient = 0;

    HbO2ClientConfig google_config;
    google_config.setClientId( msClientId );
    google_config.setLocalPort( 8080 );
    google_config.setBrowserControls( &mBrowserControls );

    HbGeneralClientConfig config;
    config.setAppName( "geny-pi" );
    config.setProtocolVersion( 1 );
    config.auth().enableGoogleAuth( google_config );
    config.presence().setKeepAliveInterval( 100 );

    mpHbClient = new HbClient( config );

    mpTTSChannel = new ClientTTSChannel();

    connect( mpHbClient, &HbClient::clientStatusChanged, this, &Client::onClientStatusChanged );
    connect( mpHbClient, &HbClient::meStatusChanged,     this, &Client::onMeStatusChanged );

    connect( mpTTSChannel, &ClientTTSChannel::ttsAdded,   this, &Client::onTTSAdded );
    connect( mpTTSChannel, &ClientTTSChannel::ttsUpdated, this, &Client::onTTSUpdated );

    connect( &mModel, &ClientModel::joinRequested, this, &Client::onJoinRequested );
    connect( &mModel, &ClientModel::loginRequested, this, &Client::onGoogleLoginRequested );
    connect( &mModel, &ClientModel::leaveRequested, this, &Client::onLeaveRequested );
    connect( &mModel, &ClientModel::ttsSent, this, &Client::onTTSSent );
    connect( &mModel, &ClientModel::ttsRemoved, this, &Client::onTTSRemoved );

    qml_engine->rootContext()->setContextProperty( "cppModel", &mModel );
}

Client::~Client()
{
    stop();
}

ClientModel * Client::model()
{
    return &mModel;
}

void Client::start( const ClientPiConfig & pi_config )
{
    HbTcpClientConfig config;
    config.setAddress( pi_config.ipAddress() );
    config.setPort( pi_config.ipPort() );
    config.setReconnectionDelay( 5000 );
    config.setBadHeaderTolerant( false );

    config.plugChannel( mpTTSChannel  );

    mTcpClient = mpHbClient->joinTcpClient( config, true );
    if( mTcpClient > 0 )
    {
        HbInfo( "Client %d started.", mTcpClient );
    }
}

void Client::stop()
{
    mTcpClient = 0;
    mpHbClient->leave();

    // Force the status to disconnected as onClientStatusChanged
    // is not called anymore when:
    // mTcpClient > 0 and status is already DISCONNECTED.
    //mModel.me()->setStatus( ClientPiUser::PIUSER_DISCONNECTED );
}

void Client::onJoinRequested( const ClientPiConfig & pi_config )
{
    start( pi_config );
}

void Client::onGoogleLoginRequested()
{
    mpHbClient->OAuthRequested( HbAuthService::AUTH_GOOGLE );
}

void Client::onLeaveRequested()
{
    stop();
}

void Client::onClientStatusChanged( networkuid client_uid, HbNetworkProtocol::ClientStatus status )
{
    HbInfo( "Status changed on client %d: %s.", client_uid, HbLatin1( HbNetworkProtocol::MetaClientStatus::toString( status ) ) );
}

void Client::onMeStatusChanged( HbNetworkProtocol::UserStatus status, ShConstHbNetworkUserInfo me_info )
{
    HbInfo( "My status changed: %s.", HbLatin1( HbNetworkProtocol::MetaUserStatus::toString( status ) ) );

    if( status == HbNetworkProtocol::USER_DISCONNECTED && mTcpClient > 0 )
    {
        // Tcp client is configured to try to reconnect.
        status = HbNetworkProtocol::USER_CONNECTING;
    }

    mModel.me()->setStatus( ( ClientPiUser::PiUserStatus ) status );
    mModel.me()->setEmail( me_info->email() );
    mModel.me()->setNickname( me_info->nickname() );
}

void Client::onTTSSent( const TTSObject & tts )
{
    mpTTSChannel->sendTTS( tts );
}

void Client::onTTSRemoved( quint64 tts_uid )
{
    mpTTSChannel->removeTTS( tts_uid );
}

void Client::onTTSAdded( QList< TTSObject > tts_objects )
{
    for( TTSObject & tts : tts_objects )
    {
        mModel.ttsModel()->addTTS( tts );
    }
}

void Client::onTTSUpdated( quint64 tts_uid, TTSObject::TTSStatus tts_status )
{
    mModel.ttsModel()->updateTTS( tts_uid, tts_status );
}

void Client::onOpenBrowser( const QUrl & url )
{
    mModel.setCurrentUrl( QUrl::fromUserInput( url.url() ) );
    mModel.setBrowserNeeded( true );
}

void Client::onCloseBrowser()
{
    mModel.setCurrentUrl( QUrl( "" ) );
    mModel.setBrowserNeeded( false );
}
