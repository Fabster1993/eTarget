import QtQuick 2.0
import "." as Widgets

Rectangle {
    height: 60
    width: parent.width
    Item {
        anchors.right: parent.right
        height: parent.height
        width: connectionText.paintedWidth + 20
        Widgets.Text {
            id: connectionText
            anchors.fill: parent
            text: device.connected ? "Connected" : "Scanning..."
            color: device.connected ? "green" : "grey"
        }
    }
}
