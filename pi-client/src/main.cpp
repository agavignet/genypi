// System
#include <iostream>
// Qt
#include <QtWidgets/QApplication>
#include <QtQml>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuickControls2/QQuickStyle>
#include <QtWebView/QtWebView>
#include <QtGui/QIcon>
// Hb
#include <core/HbApplicationHelper.h>
#include <HbLogService.h>
#include <HbLoggerOutputs.h>
// Local
#include <Client.h>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
    QApplication app(argc, argv);

    QtWebView::initialize();

    if( argc >= 2 )
    {
        pi::Client::msClientId = QString( argv[1] );
    }
    else
    {
        std::cout << "Missing args." << std::endl;
#ifdef Q_OS_ANDROID
        pi::Client::msClientId = "242660704548-2il1l2vsounnkeupc85d6bfqtbdg1qj5.apps.googleusercontent.com";
#else
        app.exit(1);
#endif
    }

    hb::tools::HbApplicationHelper::initApp( "pi", "pi.hb-io.com" );
    hb::tools::HbApplicationHelper::catchInterruptingEvents();
    hb::tools::HbApplicationHelper::initSkin( "fusion" );

    hb::log::HbLogService::processArgs( argc, argv );
    hb::log::HbLogService::outputs()->addTcpSocketOutput( "192.168.1.89", 3000 );

    qmlRegisterUncreatableType< pi::ClientModel >( "PI", 1, 0, "Model", "Model uncreatable" );
    qmlRegisterUncreatableType< pi::TTSObject >( "PI", 1, 0, "TTSObject", "TTSObject uncreatable" );
    qmlRegisterUncreatableType< pi::ClientPiUser >( "PI", 1, 0, "ClientPiUser", "ClientPiUser uncreatable" );
    qmlRegisterUncreatableType< pi::ClientPiConfig >( "PI", 1, 0, "ClientPiParameter", "ClientPiParameter uncreatable" );

    QQuickStyle::setStyle("Universal");
    
    QQmlApplicationEngine qml_engine;
    qml_engine.clearComponentCache();
    pi::Client * client = new pi::Client( &qml_engine );

    qml_engine.load( QUrl( QStringLiteral( "qrc:/qml/main.qml" ) ) );

    return app.exec();
}

