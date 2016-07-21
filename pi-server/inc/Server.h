#ifndef SERVER_H
#define SERVER_H

// Qt
#include <QtCore/QObject>
// Hb
#include <contract/HbNetworkProtocol.h>
// Local

class QThread;

namespace hb
{
    namespace network
    {
        class HbServer;
    }
}

namespace pi
{
    class ServerTTSChannel;
    class ServerTTSProcessor;

    class Server : public QObject
    {
        Q_OBJECT

    public:
        explicit Server( QObject * parent = nullptr );
        virtual ~Server();

        static QString msClientId;
        static QString msClientSecret;

    private:
        void start();
        void stop();

    private slots:
        void onServerStatusChanged( networkuid server_uid, HbNetworkProtocol::ServerStatus status );

    signals:

    private:
        hb::network::HbServer * mpHbServer;
        ServerTTSChannel * mpTTSChannel;
        ServerTTSProcessor * mpTTSProcessor;
        QThread * mpTTSProcessorThread;

    };
}

#endif // BASICNETWORKSERVERMAINWINDOW_H
