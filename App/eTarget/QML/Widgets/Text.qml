import QtQuick 2.9

Text {
    FontLoader { id: fixedFont; source: "/calibril.ttf" }
    font.pixelSize: 20
    font.family: fixedFont.name
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
}
