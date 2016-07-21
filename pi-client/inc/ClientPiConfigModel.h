#ifndef CLIENTPICONFIGMODEL_H
#define CLIENTPICONFIGMODEL_H

// Qt
#include <QtCore/QAbstractListModel>
// Local
#include <ClientPiConfig.h>

namespace pi
{
    class ClientPiConfigModel : public QAbstractListModel
    {
        Q_OBJECT
        Q_PROPERTY( ClientPiConfig * pendingPiConfig READ pendingPiConfig CONSTANT )

    public:
        enum PiConfigRoles {
            LabelRole = Qt::UserRole + 1,
            IpAddressRole,
            IpPortRole
        };

        ClientPiConfigModel(QObject * parent = nullptr);
        virtual ~ClientPiConfigModel();

        ClientPiConfig * pendingPiConfig();

        void reset();

        ClientPiConfig * piConfig( const QString & label );

        Q_INVOKABLE bool addPiConfig();
        Q_INVOKABLE void removePiConfig( const QString & label );

        int rowCount( const QModelIndex & parent = QModelIndex() ) const;
        QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    protected:
        QHash< int, QByteArray > roleNames() const;

    private:
        bool addPiConfig( const ClientPiConfig & config );
        void save();
        void load();

    private:
        ClientPiConfig mPendingPiConfig;
        QList< ClientPiConfig > mPiConfigs;
        QHash< QString, qint32 > mPiConfigIndexByLabel;
    };
}

#endif // CLIENTPICONFIGMODEL_H
