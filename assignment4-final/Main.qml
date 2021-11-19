import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.0
import QtQuick.Dialogs.qml 1.0


import com.iban 1.0

Window {

    id: cal
    visible: true
    width: 800
    height: 700

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
            height: 700

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
                    Connections {
                        target: object_call
                        onImage_bytesChange: {
                            image_process.source = image_bytes;
                        }
                    }

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
                    width: 80
                    height: 60
                }
                Button {
                    id: btn_select
                    x: 120
                    y: 100
                    text: qsTr("Select image")
                    onClicked: {
                        fileDialog.open()
                    }
                }

                Item {
                    id: spacer2
                    width: 80
                    height: 60
                }
                Button {
                    x: 250
                    y: 100
                    id: btn_start
                    text: qsTr("Start")
                    onClicked: {
                        object_call.btn_start_call(object_call.data, threadNumber.text, radiusNumber.text)
                        console.log("BTN START END!")
                    }
                }

                Item {
                    id: spacer3
                    width: 80
                    height: 60
                }
                Button {
                    x: 250
                    y: 100
                    id: btn_stop
                    text: qsTr("stop")
                    onClicked: {
                        object_call.btn_stop_call()
                        console.log("BTN STOP qml")
                    }
                }

                Item {
                    id: spacer34
                    width: 80
                    height: 60
                }
                Button {
                    x: 250
                    y: 100
                    id: btn_save
                    text: qsTr("save")
                    onClicked: {
                        saveDialog.open()
                    }
                }

                FileDialog {
                    id: saveDialog
                    visible: false
                    title: "Save Image Dialog"
                    folder: shortcuts.home
                    nameFilters: ["Image files (*.jpg *.png)", "All files (*)"]
                    selectExisting: false
                    selectMultiple: false
                    onAccepted: {
                        var urlNoProtocol = (fileUrl+"").replace('file:///', '');
                        console.log("You chose to save in: " + urlNoProtocol);
                        image_process.grabToImage(function(result) {
                            console.log(result.saveToFile(urlNoProtocol));
                        });
                    }
               }

            }

           Row {
                id: column3
                x: 0
                y: 500
                width: 800
                height: 100
                transformOrigin: Item.Center
                visible: true

                Item {
                    id: spacer77
                    width: 120
                    height: 60
                }

                Text {
                    id: element
                    x: 0
                    y: 0
                    text: qsTr("Set thread number")
                    font.pixelSize: 16
                }

                Item {
                    id: spacer24
                    width: 30
                    height: 60
                }
                FocusScope {
                    id: rootfocus1
                    x: 0
                    y: 0
                    width: 80
                    height: 20
                    property alias font: threadNumber.font
                    property alias text: threadNumber.text

                    Rectangle {
                        color: "orange"
                        height: threadNumber.cursorRectangle.height
                        width: rootfocus1.width
                        visible: true
                        y: threadNumber.cursorRectangle.y
                    }

                    TextEdit {
                        id: threadNumber
                        font.pixelSize: 16
                        text: qsTr("10")
                     }
                }

                Item {
                    id: spacer35
                    width: 30
                    height: 60
                }

                Text {
                    id: element2
                    x: 0
                    y: 0
                    text: qsTr("Radius")
                    font.pixelSize: 16
                }

                Item {
                    id: spacer8
                    width: 20
                    height: 60
                }

                FocusScope {
                    id: rootfocus
                    x: 0
                    y: 0
                    width: 80
                    height: 20
                    property alias font: radiusNumber.font
                    property alias text: radiusNumber.text

                    Rectangle {
                        color: "orange"
                        height: radiusNumber.cursorRectangle.height
                        width: rootfocus.width
                        visible: true
                        y: radiusNumber.cursorRectangle.y
                    }

                    TextEdit {
                        id: radiusNumber
                        anchors.fill: parent
                        focus: true
                        font.pixelSize: 16
                        text: qsTr("3")
                     }
                }
            }


            ProgressBar {
                id: progressBar
                x: 0
                y: 550
                width: 800
                height: 35
                value: 0
                from: 0   // C++ loop starts out with 0
                to: 10 // C++ loop ends with "Count"

                background: Rectangle {
                            anchors.left: progressBar.left
                            anchors.verticalCenter: progressBar.verticalCenter
                            implicitWidth: 50
                            implicitHeight: 6
                            color: "#e6e6e6"
                            radius: 3
                        }

                        contentItem: Rectangle {
                            anchors.left: progressBar.left
                            anchors.verticalCenter: progressBar.verticalCenter
                            width: progressBar.visualPosition * progressBar.width
                            height: progressBar.height
                            radius: 2
                            color: "#17a81a"
                            z:101
                        }

                Connections {
                    target: object_call
                    onProgressChanged: {
                        progressBar.value = progress;
                    }
                }
                Connections {
                    target: object_call
                    onFromChanged: {
                        progressBar.from = from
                        console.log(progressBar.from)
                    }
                }
                Connections {
                    target: object_call
                    onCountChanged: {
                        progressBar.to = count
                        console.log(progressBar.to)
                    }
                }
            }
        }

        Object_call {
            id: object_call
            property string data
        }

        FileDialog {
            visible: false
            id: fileDialog
            title: "Please choose image"
            folder: shortcuts.home
            nameFilters: ["Image files (*.jpg *.png)", "All files (*)"]
            onAccepted: {
                image_load.source = fileDialog.fileUrl
                object_call.data = fileDialog.fileUrl
                console.log("You chose: " + fileDialog.fileUrls)
            }
            onRejected: {
                console.log("Canceled")
            }
        }
    }
}
