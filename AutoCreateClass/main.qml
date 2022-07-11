import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import AutoCreateClass 1.0
import Qt.labs.platform 1.0
import Qt.labs.settings 1.1

Window {
    id:mainWindow
    width: 800
    height: 600

    visible: true
    title: qsTr("自动生成QT类")

    function closeFocu(obj) {
        if("focus" in obj)
            obj.focus = false;

        if("children" in obj && obj.children.length > 0) {
            var childs = obj.children;
            for(var i = 0; i < childs.length; i++) {
                var child = childs[i];
                closeFocu(child);
            }
        }
        if("data" in obj && obj.data.length > 0) {
            var datas = obj.data;
            for(var j = 0; j < datas.length; j++) {
                var data = datas[j];
                closeFocu(data);
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        onClicked: {
            mouse.accepted = false;
            closeFocu(mainWindow)
        }
    }

    AutoCreate {
        id: auto
    }

    Settings {
        id:settings
        property alias createPathFolder: folderDialog.folder
        property alias createPathText: pathtext.text
    }

    Label {
        id: pathtext
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: settings.createPathText
    }

    FolderDialog {
        id: folderDialog
        folder: settings.createPathFolder
        onAccepted: {
            createPath.text = folder.toString().slice(8);
            pathtext.text = createPath.text
        }
    }

    RowLayout {
        anchors.fill: parent
        TextScrol { 
            id: txtPreview
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2
            text: auto.previewData
            onEditingFinished: auto.previewData = text
            textReadOnly: true
            placeholderText:"//这边是预览区"
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width / 2
            Layout.bottomMargin: 50


            TextScrol {
                id: txtPars
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: parent.height / 2
                text: auto.parsData
                onEditingFinished: auto.parsData = text
                placeholderText:"//生成的属性示例\nint a;\nint b;"
            }

            ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: txtPars.bottom
                height: parent.height / 2

                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Label {
                        text: "类名"
                    }
                    TextField {
                        id: txtClassName
                        text: auto.className
                        onEditingFinished: auto.className = text
                        selectByMouse: true
                    }
                }

                CheckBox {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "属性信号是否传递值"
                    checked: auto.haveSignalValue
                    onCheckedChanged: auto.haveSignalValue = checked
                }

                CheckBox {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "是否是DLL"
                    checked: auto.isDll
                    onCheckedChanged: auto.isDll = checked
                }

                CheckBox {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "是否存在私有类"
                    checked: auto.havePrivate
                    onCheckedChanged: auto.havePrivate = checked
                }

                ComboBox {
                    anchors.horizontalCenter: parent.horizontalCenter
                    model: [ { text: "预览H文件", value: 0 }, { text: "预览Cpp文件", value: 1 }, { text: "预览Private文件", value: 2 } ]
                    textRole: "text"
                    valueRole: "value"
                    currentIndex: auto.previewIndex
                    onCurrentIndexChanged: auto.previewIndex = currentIndex
                }

                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    TextField {
                        id: createPath
                        selectByMouse: true
                    }
                    Button {
                        text: "选择生成路径"
                        onClicked: folderDialog.open()
                    }
                }

                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.fillWidth: true
                    Button {
                        text: "预览"
                        onClicked: {
                            if(txtClassName.text.trim())
                                auto.preview()
                        }
                    }

                    Button {
                        text: "生成"
                        onClicked: {
                            if(txtClassName.text.trim())
                                auto.create(createPath.text.trim())
                        }
                    }
                }
            }
        }
    }
}
