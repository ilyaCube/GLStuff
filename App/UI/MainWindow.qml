import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.11
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3

import Models 1.0

ApplicationWindow {
    id: mainWindow
    title: "OpenGL Tests"
    color: "black"
    width: 600
    height: 400
    visible: true

    OMainWindow {
        id: mainWindowModel
    }
    FileDialog {
        id: selectImageDialog
        title: "Select image"
        onAccepted: {
            surface3DModel.loadImageUrl(fileUrl)
        }
    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        RowLayout {
            Layout.alignment: Qt.AlignTop
            spacing: 10
            Button {
                id: loadImageButton
                Layout.fillWidth: true
                text: "Load image"
                onReleased: {
                    selectImageDialog.open()
                }
            }
            Button {
                id: closeImageButton
                Layout.fillWidth: true
                visible: surface3DModel.hasUploadedImage
                text: "Close image"
                onReleased: {
                    surface3DModel.loadImageUrl("")
                }
            }
        }
        OSurface3D {
            id: surface3DModel
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: surface3DModel.hasUploadedImage
            Component.onCompleted: {
                mainWindowModel.initSurface(this)
            }
            // visible: true
        }
    }
}