// Qt
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
// Hb
#include <HbLogService.h>
#include <core/HbUidGenerator.h>
// Local
#include <ServerTTSProcessor.h>

using namespace pi;
using namespace hb::tools;

const quint32 ServerTTSProcessor::msCheckIntervalSec = 1;

ServerTTSProcessor::ServerTTSProcessor()
{
}

ServerTTSProcessor::~ServerTTSProcessor()
{
    save();

    qDeleteAll( mTTSObjects );
}

void ServerTTSProcessor::init()
{
    mpTimer = new QTimer( this );
    connect( mpTimer, &QTimer::timeout, this, &ServerTTSProcessor::process );

    mpTimer->start( msCheckIntervalSec * 1000 );

    load();
}

void ServerTTSProcessor::onTTSStatusChanged()
{
    TTSObject * tts = dynamic_cast< TTSObject * >( sender() );
    if( !tts )
    {
        HbError( "Null tts object." );
    }

    emit ttsUpdated( tts->uid(), tts->status() );
}

void ServerTTSProcessor::onTTSReceived( const TTSObject & tts )
{
    HbInfo( "Wants to add %s.", HbLatin1( tts.message() ) );

    if( !tts.isValid() )
    {
        HbError( "TTSObject not valid." );
        return;
    }

    TTSObject * current = new TTSObject( tts );
    current->setUid( HbUidGenerator< quint64 >::get()->uid( true ) );
    current->setStatus( TTSObject::TTS_WAITING );

    mTTSObjects.insert( current->uid(), current );

    connect( current, &TTSObject::statusChanged, this, &ServerTTSProcessor::onTTSStatusChanged );

    emit ttsAdded( *current );
}

void ServerTTSProcessor::onTTSRemoved( quint64 tts_uid, const QString & email )
{
    HbInfo( "User %s wants to remove %ld.", HbLatin1( email ), tts_uid );
    Q_UNUSED( email ); // email can be used to check deletion rights.

    TTSObject * tts = mTTSObjects.value( tts_uid, nullptr );

    if( !tts )
    {
        HbError( "Impossible to remove unexistant TTS %ld", tts_uid );
        return;
    }

    mTTSObjects.remove( tts_uid );

    tts->setStatus( TTSObject::TTS_DELETED );
    tts->deleteLater();

}

void ServerTTSProcessor::onUserToSynchronize( const QString & email )
{
    HbInfo( "User %s to synchronize.", HbLatin1( email ) );
    QList< TTSObject > sync;
    getTTSObjects( sync );

    if( !sync.isEmpty() )
    {
        emit userSynchonized( sync, email );
    }
    else
    {
        HbInfo( "No data to synchronize." );
    }
}

void ServerTTSProcessor::process()
{
    QDateTime now = QDateTime::currentDateTime();
    static QDateTime previous_datetime = now;

    int day = now.date().dayOfWeek() - 1;

    auto it = mTTSObjects.begin();
    while( it != mTTSObjects.end() )
    {
        auto current = it++;
        TTSObject * tts = current.value();

        q_assert_ptr( tts );

        if( tts->isPeriodic() && !tts->days()[day] )
        {
            continue;
        }

        if( tts->isScheduled() )
        {
            QDateTime tts_datetime( now.date(), tts->time() );

            if( tts_datetime < previous_datetime ||
                tts_datetime > now )
            {
                continue;
            }
        }

        speak( *tts );

        if( !tts->isPeriodic() )
        {
            mTTSObjects.erase( current );
            tts->setStatus( TTSObject::TTS_DELETED );
            tts->deleteLater();
        }
    }

    previous_datetime = now;
}

void ServerTTSProcessor::save()
{
    QFile file( "tts.json" );

    if( !file.open( QIODevice::WriteOnly ) )
    {
        HbError( "Couldn't open %s file.", HbLatin1( file.fileName() ) );
        return;
    }

    QJsonArray tts_list;
    for( TTSObject* tts : mTTSObjects )
    {
        if( !tts->isPeriodic() )
        {
            // For now we only save periodic TTS.
            continue;
        }

        QJsonObject object;
        tts->writeToJson( object );
        tts_list.append( object );
    }

    QJsonDocument json(tts_list);
    file.write(json.toJson());
}

void ServerTTSProcessor::load()
{
    QFile file( "tts.json" );

    if( !file.open( QIODevice::ReadOnly ) )
    {
        HbError( "Couldn't open %s file.", HbLatin1( file.fileName() ) );
        return;
    }

    QByteArray data = file.readAll();

    QJsonDocument json(QJsonDocument::fromJson(data));

    QJsonArray tts_list = json.array();
    for( int i = 0; i < tts_list.count(); ++i )
    {
        TTSObject tts;
        tts.readFromJson( tts_list[i].toObject() );

        if( tts.isValid() )
        {
            onTTSReceived( tts );
        }
        else
        {
            HbWarning( "Invalid TTS, ignored." );
        }
        
    }
}

void ServerTTSProcessor::getTTSObjects( QList< TTSObject > & tts_objects )
{
    for( TTSObject * tts : mTTSObjects.values() )
    {
        tts_objects.append( *tts );
    }
}

void ServerTTSProcessor::speak( TTSObject & tts )
{
    tts.setStatus( TTSObject::TTS_SPEAKING );
    HbInfo( "Speaking... %s", HbLatin1( tts.message() ) );

    QProcess p;
    p.start( QDir::homePath() + "/speak.bat" );
    p.waitForStarted();
    p.waitForFinished();

    tts.setStatus( TTSObject::TTS_WAITING );
}
