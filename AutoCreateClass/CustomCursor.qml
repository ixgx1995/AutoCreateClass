import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property alias color: cursor.color
    property alias interval: timer.interval
    property bool cursorRuning: true

    id: cursor
    width: 2
    height: 1
    color: "green"
    Timer {
        id: timer
        interval: 500
        repeat: true
        running: true
        onTriggered: {
            cursor.visible = !cursor.visible
        }
    }

    onCursorRuningChanged: {
        timer.running = cursorRuning
        visible = cursorRuning
    }
}



