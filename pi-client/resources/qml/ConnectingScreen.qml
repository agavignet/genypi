import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

import PI 1.0
import "."

BaseScreen {
    id: root

    ColumnLayout {
        id: container
        width: parent.width
        anchors.centerIn: parent
        spacing: 5

        Label {
            id: message
            Layout.fillWidth: true
            text: "Waiting for Pi..."
            horizontalAlignment: Text.AlignHCenter
        }

        BusyAnimation {
            height: message.height
            Layout.fillWidth: true
            animationDurationPerItem: 800
        }

        Button {
            text: "Cancel"
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                cppModel.onLeaveRequested();
            }
        }
    }
}

