import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import PI 1.0
import "."

BaseScreen {
    property Item stack: parent
    property bool hasBack: true
    readonly property Item body: bodyArea
    property string title

    RowLayout {
        id: header
        height: backButton.implicitHeight
        width: parent.width

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        Layout.margins: 0

        Button {
            id: backButton
            visible: hasBack

            anchors.left: parent.left

            text: "Back"

            onClicked: {
                stack.pop();
            }
        }

        Label {
            Layout.fillWidth: true

            text: title
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            height: parent.implicitHeight
            width: implicitWidth
        }
    }

    Item {
        id: bodyArea

        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        anchors.topMargin: 15
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.bottomMargin: 5
    }
}
