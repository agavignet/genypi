#ifndef CLIENTTTSMODEL_H
#define CLIENTTTSMODEL_H

// Qt
#include <QtCore/QAbstractListModel>
// Local
#include <TTSObject.h>

namespace pi
{
    class ClientTTSModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        enum TTSRoles {
            NicknameRole = Qt::UserRole + 1,
            PictureRole,
            UidRole,
            StatusRole,
            MessageRole,
            TimeRole,
            PeriodicRole,
            DaysRole
        };

        ClientTTSModel(QObject * parent = nullptr);
        virtual ~ClientTTSModel();

        void reset();

        void addTTS( const TTSObject & tts );
        void updateTTS( quint64 tts_uid, TTSObject::TTSStatus tts_status );

        int rowCount( const QModelIndex & parent = QModelIndex() ) const;
        QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    protected:
        QHash< int, QByteArray > roleNames() const;

    private:
        QList< TTSObject * > mTTS;
        QHash< quint64, TTSObject * > mTTSByUid;
    };
}

#endif // CLIENTTTSMODEL_H
