// Qt
#include <QtCore/QJsonObject>
// Hb
// Local
#include <ClientPiConfig.h>

using namespace pi;

ClientPiConfig::ClientPiConfig() :
    QObject( nullptr )
{
    reset();
}

ClientPiConfig::ClientPiConfig( const ClientPiConfig & source ) : QObject()
{
    if( this != &source )
    {
        mLabel = source.mLabel;
        mIpAddress = source.mIpAddress;
        mIpPort = source.mIpPort;
    }
}

ClientPiConfig ClientPiConfig::operator=( const ClientPiConfig & source )
{
    if( this != &source )
    {
        mLabel = source.mLabel;
        mIpAddress = source.mIpAddress;
        mIpPort = source.mIpPort;
    }

    return (*this);
}

void ClientPiConfig::writeToJson( QJsonObject& object ) const
{
    object["label"] = mLabel;
    object["ip"] = mIpAddress;
    object["port"] = mIpPort;
}

void ClientPiConfig::readFromJson( const QJsonObject& object )
{
    setLabel( object["label"].toString() );
    setIpAddress( object["ip"].toString() );
    setIpPort( object["port"].toInt() );
}

void ClientPiConfig::reset()
{
    mLabel.clear();
    mIpAddress.clear();
    mIpPort = 0;
}

QString ClientPiConfig::label() const
{
    return mLabel;
}

QString ClientPiConfig::ipAddress() const
{
    return mIpAddress;
}

quint16 ClientPiConfig::ipPort() const
{
    return mIpPort;
}

void ClientPiConfig::setLabel( const QString & label )
{
    if( mLabel != label )
    {
        mLabel = label;
        emit labelChanged();
    }
}

void ClientPiConfig::setIpAddress( const QString & ip_address )
{
    if( mIpAddress != ip_address )
    {
        mIpAddress = ip_address;
        emit ipAddressChanged();
    }
}

void ClientPiConfig::setIpPort( quint16 ip_port )
{
    if( mIpPort != ip_port )
    {
        mIpPort = ip_port;
        emit ipPortChanged();
    }
}
