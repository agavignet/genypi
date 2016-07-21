import QtQuick 2.7
import QtQuick.Controls 2.0

import PI 1.0
import "."

Item {
    property Item notificationHandler

    StackView {
        id: startStack
        anchors.fill: parent

        Connections {
            target: cppModel

            onNewNotification: {
                notificationHandler.pushNotification( message );
            }
        }

        Component.onCompleted: {
            var screen = startScreen.createObject( startStack );
            startStack.push( screen );
        }

        Component {
            id: startScreen
            StartScreen {}
        }
    }
}
