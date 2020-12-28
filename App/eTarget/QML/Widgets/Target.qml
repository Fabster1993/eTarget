import QtQuick 2.0
import "." as Widgets

Rectangle {
    id: target
    property string borderColor: "black"
    property int borderWidth: 2
    property var strikePoints: []
    function addStrike(x, y) {
        console.log("X: " + x + " Y: " + y)
        var component = Qt.createComponent("StrikePoint.qml");
        if (component.status === Component.Ready) {
            strikePoints.unshift(component.createObject(target));
            console.log(x / 100)
            strikePoints[0].x = ((x / 100) * target.width / 2 + target.width / 2) - strikePoints[0].width / 2;
            strikePoints[0].y = ((-y / 100) * target.height / 2 + target.height / 2) - strikePoints[0].height / 2;
            markStrikeAt(0)
        }
    }
    function markStrikeAt(elementIndex) {
        strikePoints.forEach(element => element.color = "black")
        strikePoints[elementIndex].color = "red"
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
