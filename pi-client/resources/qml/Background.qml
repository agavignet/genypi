import QtQuick 2.7

import PI 1.0
import "."

Item {

    Rectangle {
        id: background
        width: parent.witdh
        height: parent.height

        anchors.fill: parent

        color: "transparent"

        //Image {
        //    opacity: 1
        //    anchors.fill: parent
        //    fillMode: Image.Tile
        //    source: "qrc:/img/repeated-background-3.png"
        //}
    }
}

