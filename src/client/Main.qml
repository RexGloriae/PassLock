import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Controls.Material 6.2

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("PassLock")
    Material.theme: Material.Dark

    onClosing: {
           Qt.quit();
       }

    Rectangle {
        width: parent.width
        height: parent.height

        Image {
            source: "qrc:/imgs/mainbkg.jpg"
            width: parent.width
            height: parent.height
            fillMode: Image.PreserveAspectCrop
            z: 0
        }

        StackView {
            id: stackView
            anchors.fill: parent
            initialItem: Login {}
        }

        Notification {
            id: notify
        }
    }
}
