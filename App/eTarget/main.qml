import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import "Widgets" as Widgets

ApplicationWindow {
    visible: true
    width: 460
    height: 600
    title: "eTarget"
    header:
        Widgets.AppHeader {}
    Item {
        anchors.top: parent.top
        height: parent.height / 2
        width: parent.width
        Widgets.Target {
            id: target
            anchors.centerIn: parent
            height: parent.height - 30
            width: height
        }
    }
    ListView {
        anchors.bottom: parent.bottom
        height: parent.height / 2
        width: parent.width
        model: targetIndicatorModel
        delegate: Rectangle {
            height: 40
            width: parent.width
            color: index % 2 ? "white" : "lightgrey"
            Widgets.Text {
                anchors.left: parent.left
                height: parent.height
                font.pointSize: 10
                text: "X:" + model.xPosition + " Y:" + model.yPosition
                leftPadding: 5
            }
            Image {
                function calculateRotationAngle(quadrant) {
                    switch(quadrant) {
                    case 1: return 45;
                    case 2: return -45;
                    case 3: return -135;
                    case 4: return 135;
                    }
                }
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 20
                height: parent.height - 10
                width: height
                source: "/arrow.svg"
                transformOrigin: Item.Center
                rotation: calculateRotationAngle(model.quadrant)
            }
        }
    }
}
