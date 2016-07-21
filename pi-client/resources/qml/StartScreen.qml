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
    stack: startStack

    Component {
        id: parametersScreen
        ParametersScreen {}
    }

    ListView {
        id: piListView
        anchors.fill: body

        model: cppModel.piConfigModel
        delegate: parameterDelegate
        spacing: 5
    }

    Label {
        anchors.fill: body
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "No Pi configured..."
        visible: ( piListView.count === 0 )
    }

    Button {
        anchors.bottom: body.bottom
        anchors.right: body.right
        text: "Add"

        onClicked: {
            var screen = parametersScreen.createObject( startStack );
            startStack.push( screen );
        }
    }

    Component {
        id: parameterDelegate

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

                onClicked: {
                    console.log( "Connect clicked." );
                    cppModel.onJoinRequested( LabelRole );
                }

                Menu {
                    id: menu

                    x: mouseArea.width - width
                    y: mouseArea.y + ( mouseArea.height - height ) / 2

                    MenuItem {
                        text: "Delete"

                        onTriggered: {
                            console.log( "Want to remove", LabelRole );
                            cppModel.piConfigModel.removePiConfig( LabelRole );
                        }
                    }
                }

                ColumnLayout {
                    id: entry
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    spacing: 5
                    width: parent.width

                    Label {
                        text: LabelRole
                        Layout.fillWidth: true
                    }

                    Label {
                        text: IpAddressRole + ":" + IpPortRole
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}

