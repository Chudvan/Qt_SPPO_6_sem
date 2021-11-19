import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.0
import QtQuick.Dialogs.qml 1.0

Item {

    id: cal
    visible: true
    width: 800
    height: 600

    Item {
        id: body
        x: 0
        anchors.top: parent.top

        width: parent.width
        height: childrenRect.height
        anchors.topMargin: 0

        Column {
            id: row
            x: 0
            y: 0
            width: 800
            height: 600

            Row {
                id: column
                x: 0
                y: 0
                width: 800
                height: 300

                Image {
                    id: image_load
                    x: 0
                    y: 0
                    width: 300
                    height: 300
                    //source: "qrc:/qtquickplugin/images/template_image.png"
                    fillMode: Image.PreserveAspectFit
                }

                Item {
                    id: spacer1
                    width: 120
                    height: 60
                }

                Image {
                    id: image_process
                    x: 150
                    y: 0
                    width: 300
                    height: 300
                    //source: "qrc:/qtquickplugin/images/template_image.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Row {
                id: column1
                x: 0
                y: 300
                width: 800
                height: 250
                transformOrigin: Item.Center
                visible: true

                Item {
                    id: spacer0
                    width: 120
                    height: 60
                }
                Button {
                    id: btn_select
                    x: 120
                    y: 100
                    text: qsTr("Select image")
                }

                Item {
                    id: spacer2
                    width: 120
                    height: 60
                }
                Button {
                    x: 250
                    y: 100
                    id: btn_start
                    text: qsTr("Start")
                }
            }

            ProgressBar {
                id: progressBar
                x: 0
                y: 550
                width: 800
                height: 35
                value: 0
            }
        }

        FileDialog {
            visible: false
            id: fileDialog
            title: "Please choose image"
            folder: shortcuts.home
            nameFilters: ["Image files (*.jpg *.png)", "All files (*)"]
            Component.onCompleted: visible = true
        }

        TextInput {
            id: textInput
            x: 59
            y: 486
            width: 80
            height: 20
            text: qsTr("Text Input")
            font.pixelSize: 12
        }

        TextEdit {
            id: textEdit
            x: 298
            y: 476
            width: 80
            height: 20
            text: qsTr("Text Edit")
            font.pixelSize: 12
        }

        Text {
            id: element
            x: 510
            y: 499
            text: qsTr("Text")
            font.pixelSize: 12
        }
    }
}
