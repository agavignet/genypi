import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.0

import PI 1.0
import "."

AppScreen {
    id: root
    title: "Genypi Client"
    hasBack: false

    Component {
        id: addScreen
        AddScreen {}
    }

    ListView {
        id: ttsListView
        anchors.fill: body

        model: cppModel.ttsModel
        delegate: ttsDelegate
        spacing: 5
    }

    Label {
        anchors.fill: body
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "It's empty out there..."
        visible: ( ttsListView.count === 0 )
    }

    Button {
        anchors.bottom: body.bottom
        anchors.left: body.left
        text: "Logout"

        onClicked: {
            cppModel.onLeaveRequested();
        }
    }
	
    Button {
        anchors.bottom: body.bottom
        anchors.right: body.right
        text: "Add"

        onClicked: {
            console.log( "Add TTS" );
            var screen = addScreen.createObject( stack );
            stack.push( screen );
        }
    }

    Component {
        id: ttsDelegate

        Rectangle {
            id: item
            implicitHeight: mouseArea.implicitHeight
            width: parent.width
            color: mouseArea.pressed ? Universal.baseMediumColor : Universal.baseLowColor

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                implicitHeight: entry.implicitHeight

                onPressAndHold: {
                    menu.open();
                }

                Menu {
                    id: menu

                    x: mouseArea.width - width
                    y: mouseArea.y + ( mouseArea.height - height ) / 2

                    MenuItem {
                        text: "Delete"

                        onTriggered: {
                            console.log( "Want to remove", UidRole );
                            console.log( PictureRole );
                            cppModel.removeTTS( UidRole );
                        }
                    }
                }

                RowLayout {
                    id: entry
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    spacing: 5
                    width: parent.width

                    Image {
                        source: PictureRole
                        sourceSize.width: entry.implicitHeight
                        sourceSize.height: entry.implicitHeight

                        Image {
                            visible: ( StatusRole === TTSObject.TTS_SPEAKING )
                            source: "qrc:/img/speaking.png"
                            sourceSize.width: entry.implicitHeight
                            sourceSize.height: entry.implicitHeight
                        }
                    }

                    ColumnLayout {

                        Label {
                            text: "\"" + MessageRole + "\""
                            Layout.fillWidth: true
                        }

                        RowLayout {
                            id: scheduledOutputs

                            Label {
                                text: TimeRole
                                Layout.fillWidth: true
                            }

                            Row {
                                visible: PeriodicRole
                                spacing: 5

                                Label {
                                    text: "M"
                                    color: scheduledOutputs.colorChooser( TTSObject.MONDAY )
                                }
                                Label {
                                    text: "Tu"
                                    color: scheduledOutputs.colorChooser( TTSObject.TUESDAY )
                                }
                                Label {
                                    text: "W"
                                    color: scheduledOutputs.colorChooser( TTSObject.WEDNESDAY )
                                }
                                Label {
                                    text: "Th"
                                    color: scheduledOutputs.colorChooser( TTSObject.THURSDAY )
                                }
                                Label {
                                    text: "F"
                                    color: scheduledOutputs.colorChooser( TTSObject.FRIDAY )
                                }
                                Label {
                                    text: "Sa"
                                    color: scheduledOutputs.colorChooser( TTSObject.SATURDAY )
                                }
                                Label {
                                    text: "Su"
                                    color: scheduledOutputs.colorChooser( TTSObject.SUNDAY )
                                }
                            }

                            function colorChooser( day ) {
                                if( DaysRole[day] ) {
                                    return Universal.foreground;
                                } else {
                                    return Universal.baseLowColor;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

