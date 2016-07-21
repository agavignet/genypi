#ifndef TTSREMOVECONTRACT_H
#define TTSREMOVECONTRACT_H

// Qt
// Hb
#include <HbGlobal.h>
#include <HbNetwork.h>
#include <contract/HbNetworkContract.h>
// Local

namespace pi
{
    class TTSRemoveContract : public HbNetworkContract
    {

    public:

        TTSRemoveContract();
        virtual ~TTSRemoveContract() = default;

        virtual QString toString() const;

        virtual bool read ( QDataStream & stream );
        virtual bool write( QDataStream & stream ) const;

        quint64 ttsUid() const;
        void setTTSUid( quint64 tts_uid );

    protected:
        virtual TTSRemoveContract * create() const override;

    private:
        quint64 mTTSUid;
    };
}

using pi::TTSRemoveContract;

#endif // TTSREMOVECONTRACT_H
