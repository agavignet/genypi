// Qt
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QStandardPaths>
// Hb
#include <HbLogService.h>
// Local
#include <ClientPiConfigModel.h>
#include <ClientPiConfig.h>

using namespace pi;

ClientPiConfigModel::ClientPiConfigModel( QObject * parent ) : QAbstractListModel( parent )
{
    load();
}

ClientPiConfigModel::~ClientPiConfigModel()
{
    save();
    reset();
}

ClientPiConfig * ClientPiConfigModel::pendingPiConfig()
{
    mPendingPiConfig.reset();
    return &mPendingPiConfig;
}

void ClientPiConfigModel::reset()
{
    beginResetModel();
    mPiConfigs.clear();
    mPiConfigIndexByLabel.clear();
    endResetModel();
}

ClientPiConfig * ClientPiConfigModel::piConfig( const QString & label )
{
    if( mPiConfigIndexByLabel.contains( label ) )
    {
        return &mPiConfigs[mPiConfigIndexByLabel[label]];
    }

    return nullptr;
}

bool ClientPiConfigModel::addPiConfig()
{
    return addPiConfig( mPendingPiConfig );
}

void ClientPiConfigModel::removePiConfig( const QString & label )
{
    quint32 to_remove = mPiConfigIndexByLabel.value( label, -1 );
    if( to_remove < 0 )
    {
        HbError( "Unable to find %s.", HbLatin1( label ) );
        return;
    }

    QModelIndex index_current = index( to_remove );
    QModelIndex index_parent = index_current.parent();

    beginRemoveRows( index_parent, index_current.row(), index_current.row() ); // Remove only one.

    mPiConfigs.removeAt( index_current.row() );

    endRemoveRows();

    HbInfo( "Pi config %s deleted.", HbLatin1( label ) );
}

int ClientPiConfigModel::rowCount( const QModelIndex & ) const
{
    return mPiConfigs.count();
}

QVariant ClientPiConfigModel::data( const QModelIndex & index, int role ) const
{
    if( index.row() < 0 || index.row() >= mPiConfigs.count() )
    {
        return QVariant();
    }

    if( role == LabelRole )
    {
        return mPiConfigs.at( index.row() ).label();
    }
    else if( role == IpAddressRole )
    {
        return mPiConfigs.at( index.row() ).ipAddress();
    }
    else if( role == IpPortRole )
    {
        return mPiConfigs.at( index.row() ).ipPort();
    }
    else
    {
        return QVariant();
    }
}

QHash<int, QByteArray> ClientPiConfigModel::roleNames() const
{
    QHash< int, QByteArray > roles;
    roles[LabelRole] = "LabelRole";
    roles[IpAddressRole] = "IpAddressRole";
    roles[IpPortRole] = "IpPortRole";

    return roles;
}

bool ClientPiConfigModel::addPiConfig( const ClientPiConfig & config )
{
    if( mPiConfigIndexByLabel.contains( config.label() ) )
    {
        return false;
    }

    beginInsertRows( QModelIndex(), rowCount(), rowCount() );

    mPiConfigs.push_back( config );
    mPiConfigIndexByLabel.insert( config.label(), mPiConfigs.size() - 1 );

    endInsertRows();

    HbInfo( "PiConfig %s created.", HbLatin1( config.label() ) );

    return true;
}

void ClientPiConfigModel::save()
{
    QString writable_dir = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );
    if( writable_dir.isEmpty() )
    {
        HbError( "No writable directory." );
        return;
    }

    QDir dir( writable_dir );
    if( !dir.exists() )
    {
        QDir root = QDir::root();
        if( !root.mkpath( writable_dir ) )
        {
            HbError( "Unable to create writable dir." );
            return;
        }
    }

    QFile file( dir.path() +
                QDir::separator() +
                "piconfig.json" );

    if( !file.open( QIODevice::WriteOnly ) )
    {
        HbError( "Couldn't open %s file.", HbLatin1( file.fileName() ) );
        return;
    }

    QJsonArray pi_config_list;
    for( ClientPiConfig pi_config : mPiConfigs )
    {
        QJsonObject object;
        pi_config.writeToJson( object );
        pi_config_list.append( object );
    }

    QJsonDocument json( pi_config_list );
    file.write( json.toJson() );
}

void ClientPiConfigModel::load()
{
    QFile file( QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation ) +
                QDir::separator() +
                "piconfig.json" );

    if( !file.open( QIODevice::ReadOnly ) )
    {
        HbError( "Couldn't open %s file.", HbLatin1( file.fileName() ) );
        return;
    }

    QByteArray data = file.readAll();

    QJsonDocument json(QJsonDocument::fromJson(data));

    QJsonArray pi_config_list = json.array();
    for( int i = 0; i < pi_config_list.count(); ++i )
    {
        ClientPiConfig pi_config;
        pi_config.readFromJson( pi_config_list[i].toObject() );

        addPiConfig( pi_config );

    }
}
