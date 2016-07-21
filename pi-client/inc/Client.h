#ifndef CLIENT_H
#define CLIENT_H

// Qt
#include <QtCore/QObject>
// Hb
#include <contract/HbNetworkProtocol.h>
#include <user/HbNetworkUserInfo.h>
// Local
#include <ClientModel.h>
#include <ClientBrowserControls.h>
#include <ClientPiConfig.h>

class QQmlApplicationEngine;

namespace hb
{
    namespace network
    {
        class HbClient;
    }
}

namespace pi
{
    class ClientTTSChannel;

    class Client : public QObject
    {
        Q_OBJECT

    public:
        explicit Client( QQmlApplicationEngine * qml_engine );
        virtual ~Client();

        ClientModel * model();

        static QString msClientId;

    private:
        void start( const ClientPiConfig & pi_config );
        void stop();

    public slots:
        // From network.
        void onClientStatusChanged( networkuid client_uid, HbNetworkProtocol::ClientStatus status );
        void onMeStatusChanged( HbNetworkProtocol::UserStatus status, hb::network::ShConstHbNetworkUserInfo me_info );
        // From UI.
        void onTTSSent( const TTSObject & tts );
        void onTTSRemoved( quint64 tts_uid );
        // From channel.
        void onTTSAdded( QList< TTSObject > tts_objects );
        void onTTSUpdated( quint64 tts_uid, TTSObject::TTSStatus tts_status );

        // From browser controls
        void onOpenBrowser( const QUrl & url );
        void onCloseBrowser();

    private slots:
        void onJoinRequested( const ClientPiConfig & pi_config );
        void onGoogleLoginRequested();
        void onLeaveRequested();

    private:
        hb::network::HbClient * mpHbClient;
        ClientTTSChannel * mpTTSChannel;
        ClientBrowserControls mBrowserControls;

        ClientModel mModel;

        networkuid mTcpClient;

    signals:

    };
}

#endif // BASICNETWORKUSERMAINWINDOW_H
