#ifndef TTSSENDCONTRACT_H
#define TTSSENDCONTRACT_H

// Qt
// Hb
#include <HbGlobal.h>
#include <HbNetwork.h>
#include <contract/HbNetworkContract.h>
// Local
#include <TTSObject.h>

namespace pi
{
    class TTSSendContract : public HbNetworkContract
    {

    public:

        TTSSendContract();
        virtual ~TTSSendContract() = default;

        virtual QString toString() const;

        virtual bool read ( QDataStream & stream );
        virtual bool write( QDataStream & stream ) const;

        const TTSObject & ttsObject() const;
        void setTTSObject( const TTSObject & tts );

    protected:
        virtual TTSSendContract * create() const override;

    private:
        TTSObject mTTSObject;
    };
}

using pi::TTSSendContract;

#endif // TTSSENDCONTRACT_H
