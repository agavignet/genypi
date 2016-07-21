#ifndef CLIENTPICONFIG_H
#define CLIENTPICONFIG_H

// Qt
#include <QtCore/QObject>

namespace pi
{
    class ClientPiConfig : public QObject
    {
        Q_OBJECT

        Q_PROPERTY( QString label READ label WRITE setLabel NOTIFY labelChanged )
        Q_PROPERTY( QString ipAddress READ ipAddress WRITE setIpAddress NOTIFY ipAddressChanged )
        Q_PROPERTY( quint16 ipPort READ ipPort WRITE setIpPort NOTIFY ipPortChanged )

    public:
        ClientPiConfig();
        ~ClientPiConfig() = default;
        ClientPiConfig( const ClientPiConfig & source );
        ClientPiConfig operator =( const ClientPiConfig & source );

        void writeToJson( QJsonObject& object ) const;
        void readFromJson( const QJsonObject& object );

        void reset();

        QString label() const;
        QString ipAddress() const;
        quint16 ipPort() const;

        void setLabel( const QString & label );
        void setIpAddress( const QString & ip_address );
        void setIpPort( quint16 ip_port );

    signals:
        void labelChanged();
        void ipAddressChanged();
        void ipPortChanged();

    private:
        QString mLabel;
        QString mIpAddress;
        quint16 mIpPort;

    };
}

#endif // CLIENTPICONFIG_H
