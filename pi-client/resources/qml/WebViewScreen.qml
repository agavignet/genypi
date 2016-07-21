import QtQuick 2.7
import QtWebView 1.1

import PI 1.0
import "."

BaseScreen {

    WebView {
        property bool firstLoad: true
        id: webView
        visible: !firstLoad
        anchors.fill: parent
        url: cppModel.currentUrl
        onLoadingChanged: {
            if( !loading ) {
                firstLoad = false;
            }
        }

    }

    AuthenticatingScreen {
        anchors.fill: parent
        visible: webView.firstLoad
    }
}

