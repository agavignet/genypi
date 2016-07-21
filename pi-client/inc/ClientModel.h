#ifndef MODEL_H
#define MODEL_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtQml/QQmlListProperty>
// Hb
// Local
#include <ClientPiUser.h>
#include <TTSObject.h>
#include <ClientTTSModel.h>
#include <ClientPiConfig.h>
#include <ClientPiConfigModel.h>

namespace pi
{
    class ClientModel : public QObject
    {
        Q_OBJECT
        Q_PROPERTY( ClientPiUser * me READ me CONSTANT )
        Q_PROPERTY( TTSObject * pendingTTS READ pendingTTS CONSTANT )
        Q_PROPERTY( ClientTTSModel * ttsModel READ ttsModel CONSTANT )
        Q_PROPERTY( QUrl currentUrl READ currentUrl NOTIFY currentUrlChanged )
        Q_PROPERTY( bool browserNeeded READ isBrowserNeeded NOTIFY browserNeededChanged )
        Q_PROPERTY( ClientPiConfigModel * piConfigModel READ piConfigModel CONSTANT )

    public:
        explicit ClientModel( QObject * parent = nullptr );
        virtual ~ClientModel() = default;

        ClientPiUser * me();
        TTSObject * pendingTTS();
        ClientTTSModel * ttsModel();
        ClientPiConfigModel * piConfigModel();

        QUrl currentUrl() const;
        void setCurrentUrl( const QUrl & url );

        bool isBrowserNeeded() const;
        void setBrowserNeeded( bool needed );

        Q_INVOKABLE void sendTTS();
        Q_INVOKABLE void removeTTS( quint64 tts_uid );

    public slots:
        // From UI.
        void onJoinRequested( const QString & config_label );
        void onLoginRequested();
        void onLeaveRequested();

        // From model.
        void onMeStatusChanged();

    signals:
        // To UI.
        void newNotification( const QString message );
        // To network.
        void joinRequested( const ClientPiConfig & pi_config );
        void loginRequested();
        void leaveRequested();
        void ttsSent( const TTSObject & tts );
        void ttsRemoved( quint64 tts_uid );
        void currentUrlChanged();
        void browserNeededChanged();

    private:
        ClientPiUser mMe;
        TTSObject mPendingTTS;
        ClientTTSModel mTTSModel;
        QUrl mCurrentUrl;
        bool mBrowserNeeded;
        ClientPiConfigModel mPiConfigModel;

    };
}

#endif // BASICNETWORKUSERMAINWINDOW_H
