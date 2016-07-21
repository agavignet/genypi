#ifndef SERVERTTSPROCESSOR_H
#define SERVERTTSPROCESSOR_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QList>
// Hb
// Local
#include <TTSObject.h>

class QTimer;

namespace pi
{
    class ServerTTSProcessor : public QObject
    {
        Q_OBJECT
    public:

        ServerTTSProcessor();
        virtual ~ServerTTSProcessor();

        static const quint32 msCheckIntervalSec;

    public slots:
        void init();

        // From TTS.
        void onTTSStatusChanged();

        // From channel.
        void onTTSReceived( const TTSObject & tts );
        void onTTSRemoved( quint64 tts_uid, const QString & email );
        void onUserToSynchronize( const QString & email );

    private slots:
        void process();

    signals:
        // To channel.
        void ttsAdded( const TTSObject & tts );
        void userSynchonized( QList< TTSObject > tts, QString email );
        void ttsUpdated( quint64 tts_uid, TTSObject::TTSStatus status );

    private:
        void save();
        void load();

        void getTTSObjects( QList< TTSObject > & tts_objects );
        void speak( TTSObject & tts );

        QTimer * mpTimer;
        QHash< quint64, TTSObject * > mTTSObjects;
    };
}

#endif // SERVERTTSPROCESSOR_H
