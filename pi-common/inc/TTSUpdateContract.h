#ifndef TTSUPDATECONTRACT_H
#define TTSUPDATECONTRACT_H

// Qt
// Hb
#include <HbGlobal.h>
#include <HbNetwork.h>
#include <contract/HbNetworkContract.h>
// Local
#include <TTSObject.h>

namespace pi
{
    class TTSUpdateContract : public HbNetworkContract
    {

    public:

        TTSUpdateContract();
        virtual ~TTSUpdateContract() = default;

        virtual QString toString() const;

        virtual bool read ( QDataStream & stream );
        virtual bool write( QDataStream & stream ) const;

        quint64 ttsUid() const;
        void setTTSUid( quint64 tts_uid );

        TTSObject::TTSStatus ttsStatus() const;
        void setTTSStatus( TTSObject::TTSStatus tts_status );

    protected:
        virtual TTSUpdateContract * create() const override;

    private:
        quint64 mTTSUid;
        TTSObject::TTSStatus mTTSStatus;
    };
}

using pi::TTSUpdateContract;

#endif // TTSUPDATECONTRACT_H
