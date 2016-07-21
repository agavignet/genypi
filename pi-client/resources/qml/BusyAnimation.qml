import QtQuick 2.7

import PI 1.0
import "."

Item {
    id: content

    property int nbItems: 5
    property color borderColor: "#9cff88"
    property color fillColor: "green"
    property int borderWidth: 1
    property int animationDuration: 2000
    property int animationDurationPerItem: 1000
    property int pauseDuration: 1000

    Timer {
        interval: animationDuration + pauseDuration
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            timer.start();
        }
    }

    Timer {
        id: timer
        interval: (animationDuration - animationDurationPerItem) / nbItems
        repeat: true
        triggeredOnStart: true

        property int i: 0

        onTriggered: {
            var item = repeater.itemAt(i);
            item.animate();

            i++;
            if( i === repeater.count ) {
                i = 0;
                stop();
            }
        }
    }

    Repeater {
        id: repeater
        model: nbItems
        Rectangle {
            id: item
            height: content.height
            width: height
            opacity: 0
            color: fillColor
            border.color: borderColor
            border.width: borderWidth

            function animate() {
                animation.start();
            }

            ParallelAnimation {
                id: animation

                SequentialAnimation {
                    NumberAnimation {
                        target: item
                        property: "opacity"
                        from: 0
                        to: 1
                        duration: animationDurationPerItem / 2
                        easing.type: Easing.OutQuint
                    }

                    NumberAnimation {
                        target: item
                        property: "opacity"
                        from: 1
                        to: 0
                        duration: animationDurationPerItem / 2
                        easing.type: Easing.InQuint
                    }
                }

                SequentialAnimation {
                    NumberAnimation {
                        target: item
                        property: "x"
                        from: 0
                        to: content.width
                        duration: animationDurationPerItem
                        easing.type: Easing.OutInCirc//OutInSine
                    }
                }
            }
        }
    }
}
