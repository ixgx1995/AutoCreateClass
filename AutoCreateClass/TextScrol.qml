import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: mainText
    property string backColor : "#4d4d4d"
    property string foreColor: "#FFFFFF"
    property string borderColor: "#404040"
    property int borderWidth: 1

    property alias text: txtArea.text
    property alias font: txtArea.font
    property alias textReadOnly: txtArea.readOnly
    property alias placeholderText: txtArea.placeholderText
    signal editingFinished()

    color: backColor
    border.color: borderColor
    border.width: borderWidth

    ScrollView {
        anchors.fill: parent
        contentWidth: txtArea.width;
        contentHeight: txtArea.height
        clip: true

        TextArea {
            id: txtArea
            color: foreColor

            tabStopDistance: font.pixelSize * 2
            background: Rectangle { color: backColor; border.color: borderColor; border.width: borderWidth; }
            cursorDelegate: CustomCursor { cursorRuning: txtArea.cursorVisible; color: foreColor; }
            selectByMouse: true
            selectByKeyboard: true
            width: parent.parent.width
            height: parent.parent.height
            onPaintedWidthChanged: {
                var data = paintedWidth;
                var oldData = parent.parent.width;
                width = data > oldData ? data : oldData;
                parent.contentWidth = width;
            }
            onPaintedHeightChanged: {
                var data = paintedHeight;
                var oldData = parent.parent.height;
                height = data >  oldData ? data : oldData;
                parent.contentHeight = height;
            }
            onFocusChanged: {
                cursorVisible = false
            }
            onEditingFinished: {
                focus = false;
                mainText.focus = false;
                mainText.editingFinished()
            }
        }
    }
}
