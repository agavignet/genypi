// Qt
#include <QtCore/QUrl>
// Hb
#include <HbLogService.h>
// Local
#include <ClientBrowserControls.h>

using namespace pi;

void ClientBrowserControls::onOpenBrowser( const QUrl & url )
{
    HbInfo( "Opening browser on %s", HbLatin1( url.toString() ) );
    emit openBrowser( url );
}

void ClientBrowserControls::onCloseBrowser()
{
    HbInfo( "Browser could be closed." );
    emit closeBrowser();
}
