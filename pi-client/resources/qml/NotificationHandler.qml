import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.3

import PI 1.0
import "."

Item {
    id: handler
    anchors.fill: parent
    anchors.bottomMargin: parent.height * 0.20
    anchors.topMargin: parent.height * 0.20
    z: 100 // Make sure notifications are on top of everything.

    function pushNotification( message ) {
        console.log(message)
        notifications.insert(0, { "message" : message } )
    }

    function popNotification(index) {
        notifications.remove(index);
    }

    ListModel {
        id: notifications
    }

    Component {
        id: notificationComponent

        Rectangle {
            Component.onCompleted: {
                // When item is added, it needs to use current ListView width.
                width = ListView.view.width
            }

            id: notification
            opacity: 0
            radius: height / 2
            implicitWidth: label.width
            height: label.height
            color: mouseArea.pressed ? Universal.baseMediumLowColor : Universal.baseLowColor

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                onClicked: {
                    handler.popNotification(index);
                }
            }

            Label {
                id: label
                text: message
                padding: 5
                anchors.horizontalCenter: notification.horizontalCenter
            }

            SequentialAnimation {
                running: true
                NumberAnimation {
                    target: notification
                    properties: "opacity"
                    to: 1
                    duration: 1000
                }

                NumberAnimation {
                    target: notification
                    properties: "opacity"
                    to: 0
                    duration: 9000
                }

                onRunningChanged: {
                    if (!running) {
                        handler.popNotification(notification);
                    }
                }
            }
        }
    }

    ListView {
        id: notificationArea
        model: notifications
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: contentWidth
        contentWidth: contentItem.childrenRect.width
        interactive: false
        verticalLayoutDirection: ListView.BottomToTop

        delegate: notificationComponent

        onWidthChanged: {
            // Each time listview width changes, we update width
            // of all items to show them at the same width.
            for(var i = 0; i < contentItem.children.length; i++) {
                contentItem.children[i].width = width;
            }
        }

        // Debug purpose.
        //Rectangle {
        //    anchors.fill: parent
        //    opacity: 0.50
        //    color: "yellow"
        //}
    }
}


