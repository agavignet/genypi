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
            id: messageField
            inputMethodHints: Qt.ImhLatinOnly | Qt.ImhLowercaseOnly
            Layout.fillWidth: true
            placeholderText: "Text to speech"
        }

        Switch {
            id: isScheduled
            text: "Scheduled?"
            Layout.fillWidth: true
        }

        ColumnLayout {
            id: scheduledInputs
            visible: isScheduled.position === 1.0

            Layout.alignment: Qt.AlignHCenter

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Label {
                    text: "Hour:"
                }

                Tumbler {
                    id: hour
                    model: 24
                    visibleItemCount: 5
                }

                Tumbler {
                    id: minute
                    model: 60
                    visibleItemCount: 5
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Button {
                    id: repetitionMonday
                    text: "M"
                    checkable: true
                }
                Button {
                    id: repetitionTuesday
                    text: "Tu"
                    checkable: true
                }
                Button {
                    id: repetitionWednesday
                    text: "W"
                    checkable: true
                }
                Button {
                    id: repetitionThursday
                    text: "Th"
                    checkable: true
                }
                Button {
                    id: repetitionFriday
                    text: "F"
                    checkable: true
                }
                Button {
                    id: repetitionSaturday
                    text: "Sa"
                    checkable: true
                }
                Button {
                    id: repetitionSunday
                    text: "Su"
                    checkable: true
                }
            }

            PropertyAnimation {
                target: scheduledInputs;
                properties: "opacity";
                easing.type: Easing.InOutElastic;
                easing.amplitude: 2.0;
                easing.period: 1.5 }
        }
    }

    Button {
        id: sendButton
        text: "Send"

        anchors.bottom: body.bottom
        anchors.right: body.right

        onClicked: {

            var tts = cppModel.pendingTTS

            tts.message = messageField.text;

            if( isScheduled.checked ) {
                tts.setTime( hour.currentIndex, minute.currentIndex );
            }

            var days = [ repetitionMonday.checked,
                         repetitionTuesday.checked,
                         repetitionWednesday.checked,
                         repetitionThursday.checked,
                         repetitionFriday.checked,
                         repetitionSaturday.checked,
                         repetitionSunday.checked,
                       ];
            tts.days = days;

            cppModel.sendTTS();

            stack.pop();
        }
    }
}

