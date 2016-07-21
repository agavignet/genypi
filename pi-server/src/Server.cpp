// Qt
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
// Hb
#include <HbGlobal.h>
#include <HbLogService.h>
#include <config/HbO2ServerConfig.h>
#include <config/peer/HbGeneralServerConfig.h>
#include <HbServer.h>
// Local
#include <Server.h>
#include <Protocol.h>
#include <ServerTTSChannel.h>
#include <ServerTTSProcessor.h>

using namespace hb::log;
using namespace hb::network;
using namespace pi;

QString Server::msClientId;
QString Server::msClientSecret;

Server::Server( QObject * parent ) :
    QObject( parent )
{
    qRegisterMetaType< TTSObject >( "TTSObject" );
    qRegisterMetaType< QList< TTSObject > >( "QList< TTSObject >" );
    qRegisterMetaType< TTSObject::TTSStatus >( "TTSObject::TTSStatus" );

    HbO2ServerConfig google_config;
    google_config.setClientId( msClientId );
    google_config.setClientSecret( msClientSecret );

    HbGeneralServerConfig config;
    config.setAppName("geny-pi");
    config.setProtocolVersion( 1 );
    config.auth().setAuthMaxTries( 3 );
    config.auth().setAuthTimeout( 60 );
    config.auth().enableGoogleAuth( google_config );
    config.presence().setWarningAliveThreshold( 300 );
    config.presence().setKickAliveThreshold( 400 );

    mpHbServer = new HbServer( config );
    mpTTSChannel = new ServerTTSChannel();
    mpTTSProcessor = nullptr;
    mpTTSProcessorThread = nullptr;

    connect( mpHbServer, &HbServer::serverStatusChanged, this, &Server::onServerStatusChanged );

    start();
}

Server::~Server()
{
    stop();
    delete mpTTSChannel;
    if( mpTTSProcessorThread )
    {
        // Will delete itself and mpTTSProcessor.
        mpTTSProcessorThread->quit();
        mpTTSProcessorThread->wait();        
    }
}

void Server::start()
{
    HbTcpServerConfig config;
    config.setAddress(QHostAddress::Any);
    config.setPort( 40000 );
    config.setMaxUsersPerThread( 1 );
    config.setBadHeaderTolerant( false );
    config.plugChannel( mpTTSChannel );

    networkuid server_uid = mpHbServer->joinTcpServer( config, true );
    if( server_uid > 0 )
    {
        mpTTSProcessor = new ServerTTSProcessor();
        mpTTSProcessorThread = new QThread();

        mpTTSProcessor->moveToThread( mpTTSProcessorThread );

        connect( mpTTSProcessorThread, &QThread::started, mpTTSProcessor, &ServerTTSProcessor::init );
        connect( mpTTSProcessorThread, &QThread::finished, mpTTSProcessor, &ServerTTSProcessor::deleteLater );
        connect( mpTTSProcessorThread, &QThread::finished, mpTTSProcessorThread, &ServerTTSProcessor::deleteLater );

        connect( mpTTSProcessor, &ServerTTSProcessor::ttsAdded, mpTTSChannel, &ServerTTSChannel::onTTSAdded );
        connect( mpTTSProcessor, &ServerTTSProcessor::userSynchonized, mpTTSChannel, &ServerTTSChannel::onUserSynchonized );
        connect( mpTTSProcessor, &ServerTTSProcessor::ttsUpdated, mpTTSChannel, &ServerTTSChannel::onTTSUpdated );
        connect( mpTTSChannel, &ServerTTSChannel::ttsReceived, mpTTSProcessor, &ServerTTSProcessor::onTTSReceived );
        connect( mpTTSChannel, &ServerTTSChannel::ttsRemoved, mpTTSProcessor, &ServerTTSProcessor::onTTSRemoved );
        connect( mpTTSChannel, &ServerTTSChannel::userToSynchonize, mpTTSProcessor, &ServerTTSProcessor::onUserToSynchronize );

        mpTTSProcessorThread->start();

        HbInfo( "Server %d started.", server_uid );
    }
    else
    {
        qApp->exit(1);
    }
}

void Server::stop()
{
    mpHbServer->leave();
}

void Server::onServerStatusChanged( networkuid server_uid, HbNetworkProtocol::ServerStatus status )
{
    HbInfo( "Status changed on server %d: %s.", server_uid, HbLatin1( HbNetworkProtocol::MetaServerStatus::toString( status ) ) );
}
