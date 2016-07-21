#ifndef PIUSER_H
#define PIUSER_H

// Qt
#include <QtCore/QString>
#include <QtCore/QDataStream>
#include <QtCore/QObject>
// Hb
// Local
#include <HbNetwork.h>

namespace pi
{
    class PiUser : public QObject
    {
        Q_OBJECT
        Q_PROPERTY( const QString & email READ email NOTIFY emailChanged )
        Q_PROPERTY( const QString & nickname READ nickname NOTIFY nicknameChanged )
        Q_PROPERTY( const QString & picture READ picture NOTIFY pictureChanged )

        friend QDataStream & operator<<( QDataStream & stream, const PiUser & user );
        friend QDataStream & operator>>( QDataStream & stream, PiUser & user );

    public:

        PiUser();
        ~PiUser() = default;
        PiUser( const PiUser & source );
        PiUser operator = (const PiUser & source);

        void writeToJson( QJsonObject& object ) const;
        void readFromJson( const QJsonObject& object );

        const QString & email    () const;
        const QString & nickname () const;
        const QString & picture  () const;

        void setEmail    ( const QString & email );
        void setNickname ( const QString & nickname );
        void setPicture  ( const QString & picture );

        QString toString() const;

    signals:
        void emailChanged( const QString & email );
        void nicknameChanged( const QString & nickname );
        void pictureChanged( const QString & picture );

    private:
        QString mEmail;
        QString mNickname;
        QString mPicture;
    };

    QDataStream & operator<<( QDataStream & stream, const PiUser & user );
    QDataStream & operator>>( QDataStream & stream, PiUser & user );
}

#endif // PIUSER_H
