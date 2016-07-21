import QtQuick 2.7

import PI 1.0
import "."

pragma Singleton

QtObject {
    property color textColor: "white"
    property int textNormalSize: 12
    property int textTitleSize: 20
    property color itemBackgroundColor: "#282828"
    property color itemBackgroundColorHover: "#333333"
    property color itemBorderColor: "#333333"
    property color itemBorderColorHover: "white"
    property int borderWidth: 1
}

