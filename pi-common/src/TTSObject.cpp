// Qt
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
// Hb
#include <HbLogService.h>
// Local
#include <TTSObject.h>

using namespace pi;

TTSObject::TTSObject() :
    QObject()
{
    mDays = { false, false, false, false, false, false, false }; // Seven days.
    mUid = 0;
    mStatus = TTS_UNDEFINED;
}

TTSObject::~TTSObject()
{

}

TTSObject::TTSObject( const TTSObject & source ) :
    TTSObject()
{
    if( this != &source )
    {
        mOwner = source.mOwner;
        mUid = source.mUid;
        mStatus = source.mStatus;
        mMessage = source.mMessage;
        mTime = source.mTime;
        mDays = source.mDays;
    }
}

TTSObject & TTSObject::operator=( const TTSObject & source )
{
    if( this != &source )
    {
        mOwner = source.mOwner;
        mUid = source.mUid;
        mStatus = source.mStatus;
        mMessage = source.mMessage;
        mTime = source.mTime;
        mDays = source.mDays;
    }

    return ( *this );
}

void TTSObject::writeToJson( QJsonObject& object ) const
{
    QJsonObject owner;
    QJsonArray days;

    for( bool day : mDays )
    {
        days.append( QJsonValue( day ) );
    }

    mOwner.writeToJson( owner );
    object["owner"] = owner;
    object["uid"] = (int) mUid;
    object["status"] = (int) mStatus;
    object["message"] = mMessage;
    object["time"] = mTime.toString( "HH:mm" );
    object["days"] = days;
}

void TTSObject::readFromJson( const QJsonObject& object )
{
    mOwner.readFromJson( object["owner"].toObject() );
    setUid( object["uid"].toInt() );
    setStatus( TTS_WAITING ); // Loaded TTS must always be in waiting status.
    setMessage( object["message"].toString() );
    setTime( QTime::fromString( object["time"].toString(), "HH:mm" ));
    QJsonArray days_array = object["days"].toArray();

    QList< bool > days;
    for( QJsonValue value : days_array )
    {
        days.append( value.toBool() );
    }

    setDays( days );
}

QString TTSObject::toString() const
{
    return QString( "owner={%1}, status=%2, message=%3, is_scheduled=%4, time=%5, M=%6, T=%7, W=%8, T=%9, F=%10" )
            .arg( mOwner.toString() )
            .arg( TTSObject::MetaTTSStatus::toString( mStatus ) )
            .arg( mMessage )
            .arg( isScheduled() )
            .arg( mTime.toString("HH:mm") )
            .arg( mDays.at( MONDAY ) )
            .arg( mDays.at( TUESDAY ) )
            .arg( mDays.at( WEDNESDAY ) )
            .arg( mDays.at( THURSDAY ) )
            .arg( mDays.at( FRIDAY ) );
}

bool TTSObject::isValid() const
{
    if( mMessage.isEmpty() )
    {
        HbError( "TTSObject not valid: empty message." );
        return false;
    }

    if( isPeriodic() && !isScheduled() )
    {
        HbError( "TTSObject not valid: periodic not scheduled." );
        return false;
    }

    if( mDays.size() != SUNDAY + 1 )
    {
        HbError( "Bad days size." );
        return false;
    }

    return true;
}

quint64 TTSObject::uid() const
{
    return mUid;
}

PiUser & TTSObject::owner()
{
    return mOwner;
}

const PiUser & TTSObject::owner() const
{
    return mOwner;
}

TTSObject::TTSStatus TTSObject::status() const
{
    return mStatus;
}

bool TTSObject::isScheduled() const
{
    return !mTime.isNull();
}

bool TTSObject::isPeriodic() const
{
    return mDays.contains( true );
}

const QString & TTSObject::message() const
{
    return mMessage;
}

QString TTSObject::messageExtract() const
{
    if( mMessage.size() <= 30 )
    {
        return mMessage;
    }

    return mMessage.left( 27 ).append( "..." );
}

const QTime & TTSObject::time() const
{
    return mTime;
}

const QList<bool> & TTSObject::days() const
{
    return mDays;
}

void TTSObject::setUid( quint64 uid )
{
    mUid = uid;
}

void TTSObject::setStatus( TTSStatus status )
{
    if( mStatus != status )
    {
        HbInfo( "Status changed from %s to %s.",
                HbLatin1( TTSObject::MetaTTSStatus::toString( mStatus )),
                HbLatin1( TTSObject::MetaTTSStatus::toString( status )));

        mStatus = status;
        emit statusChanged();
    }
}

void TTSObject::setMessage( const QString & message )
{
    if( mMessage != message )
    {
        mMessage = message;
        emit messageChanged();
    }
}

void TTSObject::setTime( const QTime & time )
{
    if( mTime != time )
    {
        mTime = time;
        emit timeChanged();
    }
}

void TTSObject::setDays( const QList< bool > & days )
{
    mDays = days;
    emit daysChanged();
}

void TTSObject::setTime( qint32 hours, qint32 minutes )
{
    mTime.setHMS( hours, minutes, 0 );
}

void TTSObject::reset()
{
    mUid = 0;
    mMessage.clear();
    mTime = QTime();
    mDays = { false, false, false, false, false, false, false };
}

namespace pi
{
    QDataStream & operator<<( QDataStream & stream, const TTSObject & tts )
    {
        stream << tts.mOwner;
        stream << tts.mUid;
        stream << ( quint8 ) tts.mStatus;
        stream << tts.mMessage;
        stream << tts.mTime;
        stream << tts.mDays;

        return stream;
    }

    QDataStream & operator>>( QDataStream & stream, TTSObject & tts )
    {
        quint8 status;
        stream >> tts.mOwner;
        stream >> tts.mUid;
        stream >> status;
        stream >> tts.mMessage;
        stream >> tts.mTime;
        stream >> tts.mDays;

        tts.mStatus = ( TTSObject::TTSStatus ) status;

        return stream;
    }
}
