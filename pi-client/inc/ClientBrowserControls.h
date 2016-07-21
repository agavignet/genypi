#ifndef CLIENTBROWSERCONTROLS_H
#define CLIENTBROWSERCONTROLS_H

// Qt
// Hb
#include <IHbLinkBrowserControls.h>

namespace pi
{
    class ClientBrowserControls : public hb::link::IHbLinkBrowserControls
    {
        Q_OBJECT

    public slots:
        virtual void onOpenBrowser( const QUrl & url );
        virtual void onCloseBrowser();

    signals:
        void openBrowser( const QUrl & url );
        void closeBrowser();
    };
}


#endif // CLIENTBROWSERCONTROLS_H
