// Qt
// Hb
#include <HbLogService.h>
// Local
#include <ClientTTSModel.h>

using namespace pi;

ClientTTSModel::ClientTTSModel( QObject * parent ) : QAbstractListModel( parent )
{

}

ClientTTSModel::~ClientTTSModel()
{
    reset();
}

void ClientTTSModel::reset()
{
    beginResetModel();
    qDeleteAll( mTTS );
    mTTS.clear();
    mTTSByUid.clear();
    endResetModel();
}

void ClientTTSModel::addTTS( const TTSObject & tts )
{
    beginInsertRows( QModelIndex(), rowCount(), rowCount() );

    TTSObject * current = new TTSObject( tts );
    mTTS.push_back( current );
    mTTSByUid.insert( tts.uid(), current );

    endInsertRows();

    HbInfo( "TTS %ld created.", current->uid() );
}

void ClientTTSModel::updateTTS( quint64 tts_uid, TTSObject::TTSStatus tts_status )
{
    TTSObject * tts = mTTSByUid.value( tts_uid, nullptr );
    q_assert_ptr( tts );

    QModelIndex index_current = index( mTTS.indexOf( tts ) );
    QModelIndex index_parent = index_current.parent();

    if( tts_status == TTSObject::TTS_WAITING ||
        tts_status == TTSObject::TTS_SPEAKING )
    {
        tts->setStatus( tts_status );
        emit dataChanged( index_current, index_current );
    }
    else
    {
        beginRemoveRows( index_parent, index_current.row(), index_current.row() ); // Remove only one.

        mTTS.removeAt( index_current.row() );
        mTTSByUid.remove( tts->uid() );
        delete tts;

        endRemoveRows();

        HbInfo( "TTS %ld deleted.", tts->uid() );
    }
}

int ClientTTSModel::rowCount( const QModelIndex & ) const
{
    return mTTS.count();
}

QVariant ClientTTSModel::data( const QModelIndex & index, int role ) const
{
    if( index.row() < 0 || index.row() >= mTTS.count() )
    {
        return QVariant();
    }

    const TTSObject * tts = mTTS.at( index.row() );
    q_assert_ptr( tts );

    if( role == NicknameRole )
    {
        return tts->owner().nickname();
    }
    else if( role == PictureRole )
    {
        return tts->owner().picture();
    }
    else if( role == UidRole )
    {
        return tts->uid();
    }
    else if( role == StatusRole )
    {
        return tts->status();
    }
    else if( role == MessageRole )
    {
        return tts->messageExtract();
    }
    else if( role == TimeRole && tts->isScheduled() )
    {
        return tts->time().toString( "HH:mm" );
    }
    else if( role == PeriodicRole )
    {
        return tts->isPeriodic();
    }
    else if( role == DaysRole )
    {
        return QVariant::fromValue< QList< bool > >( tts->days() );
    }
    else
    {
        return QVariant();
    }
}

QHash<int, QByteArray> ClientTTSModel::roleNames() const
{
    QHash< int, QByteArray > roles;
    roles[NicknameRole] = "NicknameRole";
    roles[PictureRole] = "PictureRole";
    roles[UidRole] = "UidRole";
    roles[StatusRole] = "StatusRole";
    roles[MessageRole] = "MessageRole";
    roles[TimeRole] = "TimeRole";
    roles[PeriodicRole] = "PeriodicRole";
    roles[DaysRole] = "DaysRole";

    return roles;
}
