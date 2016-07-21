#ifndef TTSOBJECT_H
#define TTSOBJECT_H

// Qt
#include <QtCore/QTime>
#include <QtCore/QBitArray>
#include <QtCore/QDataStream>
// Hb
#include <HbGlobal.h>
#include <HbNetwork.h>
#include <contract/HbNetworkContract.h>
// Local
#include <PiUser.h>

namespace pi
{
    class TTSObject : public QObject
    {
        friend QDataStream & operator<<( QDataStream & stream, const TTSObject & tts );
        friend QDataStream & operator>>( QDataStream & stream, TTSObject & tts );

        Q_OBJECT
        Q_ENUMS_HANDLER( TTSObject )
        Q_ENUMS( TTSStatus )

        Q_PROPERTY( TTSStatus status READ status WRITE setStatus NOTIFY statusChanged )
        Q_PROPERTY( QString message READ message WRITE setMessage NOTIFY messageChanged )
        Q_PROPERTY( QList<bool> days READ days WRITE setDays NOTIFY daysChanged )

    public:

        enum TTSStatus : quint8
        {
            TTS_UNDEFINED = 0,
            TTS_WAITING,
            TTS_SPEAKING,
            TTS_DELETED
        };
        Q_META_ENUMS( TTSStatus )

        enum Day : quint8
        {
            MONDAY = 0,
            TUESDAY,
            WEDNESDAY,
            THURSDAY,
            FRIDAY,
            SATURDAY,
            SUNDAY
        };
        Q_ENUM( Day )

        TTSObject();
        virtual ~TTSObject();
        TTSObject( const TTSObject & source );
        TTSObject & operator=( const TTSObject & source );

        void writeToJson( QJsonObject& object ) const;
        void readFromJson( const QJsonObject& object );

        QString toString() const;
        bool isValid() const;

        PiUser & owner();
        const PiUser & owner() const;
        quint64 uid() const;
        TTSStatus status() const;
        bool isScheduled() const;
        bool isPeriodic() const;
        const QString & message() const;
        QString messageExtract() const;
        const QTime & time() const;
        const QList<bool> & days() const;

        void setUid( quint64 uid );
        void setStatus( TTSStatus status );
        void setMessage( const QString & message );
        void setTime( const QTime & time );
        void setDays( const QList<bool> & days );

        Q_INVOKABLE void setTime( qint32 hours, qint32 minutes );

        void reset();
        
    signals:
        void statusChanged();
        void messageChanged();
        void timeChanged();
        void daysChanged();

    private:
        PiUser mOwner;
        quint64 mUid;
        TTSStatus mStatus;
        QString mMessage;
        QTime mTime;
        QList<bool> mDays;
    };
}

#endif // TTSOBJECT_H
