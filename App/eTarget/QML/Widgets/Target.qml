import QtQuick 2.0
import "." as Widgets

Rectangle {
    id: target
    property string borderColor: "black"
    property int borderWidth: 2
    function addStrike(x, y) {
        console.log("X: " + x + " Y: " + y)
        var component = Qt.createComponent("StrikePoint.qml");
        if (component.status === Component.Ready) {
            var strikePoint = component.createObject(target);
            strikePoint.x = x + target.width / 2;
            strikePoint.y = -y + target.height / 2;
        }
    }
    radius: height / 2
    border.color: borderColor
    border.width: borderWidth
    Rectangle {
        anchors.centerIn: parent
        height: parent.height * 4 / 5
        width: height
        radius: height / 2
        border.color: borderColor
        border.width: 2
    }
    Rectangle {
        anchors.centerIn: parent
        height: parent.height * 3 / 5
        width: height
        radius: height / 2
        border.color: borderColor
        border.width: borderWidth
    }
    Rectangle {
        anchors.centerIn: parent
        height: parent.height * 2 / 5
        width: height
        radius: height / 2
        border.color: borderColor
        border.width: borderWidth
    }
    Rectangle {
        anchors.centerIn: parent
        height: parent.height * 1 / 5
        width: height
        radius: height / 2
        border.color: borderColor
        border.width: borderWidth
    }
}
