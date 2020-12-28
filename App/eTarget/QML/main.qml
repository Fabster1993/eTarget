import QtQuick 2.9
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
        id: strikeListView
        property int selectedItem: 0
        anchors.bottom: parent.bottom
        height: parent.height / 2
        width: parent.width
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        model: targetIndicatorModel
        delegate: Rectangle {
            height: strikeListView.selectedItem == index ? 60 : 40
            width: strikeListView.width
            color: index % 2 ? "white" : "lightgrey"
            Widgets.Text {
                anchors.left: parent.left
                height: parent.height
                font.pixelSize: height / 3
                text: model.score + " Points | Distance to target: " + model.radius + "mm"
                leftPadding: 5
            }
            Image {
                function calculateRotationAngle(x, y) {
                    if(y > 0)
                        return (Math.atan(x/y) * 180 / Math.PI) + 180;
                    else
                        return (Math.atan(x/y) * 180 / Math.PI)
                }
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 20
                height: parent.height - 10
                width: height
                source: model.score !== 10 ? "/arrow.svg" :"/target.svg"
                transformOrigin: Item.Center
                rotation: model.score !== 10 ? calculateRotationAngle(model.xPosition, model.yPosition) : 0
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    strikeListView.selectedItem = index
                    target.markStrikeAt(index)
                }
            }
        }
        Connections {
            target: targetIndicatorModel
            function onStrikeAdded(x, y)
            {
                strikeListView.selectedItem = 0
                target.addStrike(x, y)
            }
        }
    }
}
