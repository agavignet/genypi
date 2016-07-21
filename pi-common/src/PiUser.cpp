// Qt
#include <QtCore/QJsonObject>
// Hb
// Local
#include <PiUser.h>

using namespace pi;

PiUser::PiUser()
{
}

PiUser::PiUser( const PiUser & source ) : QObject()
{
    if( this != &source )
    {
        mEmail = source.mEmail;
        mNickname = source.mNickname;
        mPicture = source.mPicture;
    }
}

PiUser PiUser::operator=( const PiUser & source )
{
    if( this != &source )
    {
        mEmail = source.mEmail;
        mNickname = source.mNickname;
        mPicture = source.mPicture;
    }

    return (*this);
}

void PiUser::writeToJson( QJsonObject& object ) const
{
    object["email"] = mEmail;
    object["nickname"] = mNickname;
    object["picture"] = mPicture;
}

void PiUser::readFromJson( const QJsonObject& object )
{
    setEmail( object["email"].toString() );
    setNickname( object["nickname"].toString() );
    setPicture( object["picture"].toString() );
}

const QString & PiUser::email() const
{
    return mEmail;
}

const QString & PiUser::nickname() const
{
    return mNickname;
}

const QString & PiUser::picture() const
{
    return mPicture;
}

void PiUser::setEmail( const QString & email )
{
    if( mEmail != email )
    {
        mEmail = email;
        emit emailChanged( mEmail );
    }
}

void PiUser::setNickname( const QString & nickname )
{
    if( mNickname != nickname )
    {
        mNickname = nickname;
        emit nicknameChanged( nickname );
    }
}

void PiUser::setPicture( const QString & picture )
{
    if( mPicture != picture )
    {
        mPicture = picture;
        emit pictureChanged( mPicture );
    }
}

QString PiUser::toString() const
{
    return QString( "email=%1, nickname=%2" )
            .arg( mEmail )
            .arg( mNickname );
}

namespace pi
{
    QDataStream & operator <<( QDataStream & stream, const PiUser & user )
    {
        stream << user.mEmail;
        stream << user.mNickname;
        stream << user.mPicture;

        return stream;
    }

    QDataStream & operator >>( QDataStream & stream, PiUser & user )
    {
        stream >> user.mEmail;
        stream >> user.mNickname;
        stream >> user.mPicture;

        return stream;
    }
}


