import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import PI 1.0
import "."

AppScreen {
    id: root
    title: "Genypi Client"

    ColumnLayout {
        Layout.fillHeight: false
        Layout.fillWidth: true
        Layout.margins: 0

        anchors.top: body.top
        anchors.left: body.left
        anchors.right: body.right

        TextField {
            id: labelField
            Layout.fillWidth: true
            placeholderText: "Label"
        }

        TextField {
            id: ipField
            Layout.fillWidth: true
            placeholderText: "Ip or url"
        }

        RowLayout {

            Label {
                text: "Ip port:"
            }

            TextField {
                id: portField
                inputMethodHints: Qt.ImhLatinOnly | Qt.ImhLowercaseOnly
                Layout.fillWidth: true

                validator: IntValidator {
                    bottom: 0
                    top: 65535
                }
            }
        }
    }

    Button {
        id: sendButton
        text: "Add"

        anchors.bottom: body.bottom
        anchors.right: body.right

        onClicked: {
            var ok = true;

            if ( labelField.text.length === 0 ) {
                notificationHandler.pushNotification( "Empty label" );
                ok = false;
            }

            if ( ipField.text.length === 0 ) {
                notificationHandler.pushNotification( "Empty ip/url" );
                ok = false;
            }

            if ( !portField.acceptableInput ) {
                notificationHandler.pushNotification( "Bad port" );
                ok = false;
            }

            if (!ok) {
                return;
            }

            var parameter = cppModel.piConfigModel.pendingPiConfig;
            parameter.label = labelField.text;
            parameter.ipAddress = ipField.text;
            parameter.ipPort = portField.text;

            if( !cppModel.piConfigModel.addPiConfig() ) {
                notificationHandler.pushNotification( "Already exists" );
            } else {
                startStack.pop();
            }
        }
    }
}

