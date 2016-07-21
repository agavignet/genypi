#ifndef TTSADDCONTRACT_H
#define TTSADDCONTRACT_H

// Qt
// Hb
#include <HbGlobal.h>
#include <HbNetwork.h>
#include <contract/HbNetworkContract.h>
// Local
#include <TTSObject.h>

namespace pi
{
    class TTSAddContract : public HbNetworkContract
    {

    public:

        TTSAddContract();
        virtual ~TTSAddContract() = default;

        virtual QString toString() const;

        virtual bool read ( QDataStream & stream );
        virtual bool write( QDataStream & stream ) const;

        const QHash< quint64, TTSObject > & ttsObjects() const;
        void addTTSObject( const TTSObject & tts );

    protected:
        virtual TTSAddContract * create() const override;

    private:
        QHash< quint64, TTSObject > mTTSObjectsByUid;
    };
}

using pi::TTSAddContract;

#endif // TTSADDCONTRACT_H
