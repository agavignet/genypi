// System
#include <iostream>
// Qt
#include <QtWidgets/QApplication>
#include <QtCore/QString>
// Hb
#include <core/HbApplicationHelper.h>
#include <HbLogService.h>
#include <HbLoggerOutputs.h>
// Local
#include <Server.h>
#include <TTSObject.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if( argc >= 3 )
    {
        pi::Server::msClientId     = QString( argv[1] );
        pi::Server::msClientSecret = QString( argv[2] );
    }
    else
    {
        std::cout << "Missing args." << std::endl;
        app.exit(1);
    }

    hb::tools::HbApplicationHelper::initApp( "pi", "pi.hb-io.com" );
    hb::tools::HbApplicationHelper::catchInterruptingEvents();
    hb::log::HbLogService::processArgs( argc, argv );
    hb::log::HbLogService::outputs()->addConsoleOutput();

    pi::Server server;

    return app.exec();
}
