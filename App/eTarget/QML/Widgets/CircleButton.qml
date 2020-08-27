import QtQuick 2.0

Rectangle {
    property string iconSource;
    signal buttonReleased()
    width: 40
    height: width
    border.color: "black"
    border.width: 2
    radius: height / 2
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onReleased: if(containsMouse) buttonReleased()
    }
    Image {
        anchors.centerIn: parent
        width: parent.width / 2
        height: parent.height / 2
        source: iconSource
    }

}
