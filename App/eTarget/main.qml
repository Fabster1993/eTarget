import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 460
    height: 600
    title: "eTarget"
    RowLayout {
        anchors.fill: parent
        Rectangle {
            border.color: "black"
            Layout.fillWidth: true
            height: 40
            TextInput {
                readOnly: false
                anchors.fill: parent
                onAccepted:
                {
                    console.log(text)
                    clear()
                }
            }
        }
    }
}
