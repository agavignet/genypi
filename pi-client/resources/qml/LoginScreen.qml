import QtQuick 2.7
import QtQuick.Controls 2.0

import PI 1.0
import "."

BaseScreen {
    id: root

    Button {
        text: "Login with Google"
        width: parent.width * 0.80
        anchors.centerIn: parent

        onClicked: {
            cppModel.onLoginRequested();
        }
    }
}

