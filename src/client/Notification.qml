import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Controls.Material 6.2

Rectangle {
    id: notification
    width: 300
    height: 60
    radius: 10
    color: "#333" // Dark background
    opacity: 0
    z: 100
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.margins: 20

    // Simple shadow effect using layer
    layer.enabled: true
    layer.samplerName: "gaussianBlur"

    Row {
        spacing: 10
        anchors.fill: parent
        anchors.margins: 10
        Image {
            id: icon
            source: "qrc:/icons/info.png"
            width: 24
            height: 24
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            id: messageText
            color: "white"
            font.pixelSize: 16
            anchors.verticalCenter: parent.verticalCenter
            wrapMode: Text.Wrap
            text: "Notification Text"
        }
    }

    Behavior on opacity {
        NumberAnimation { duration: 300 }
    }

    function show(message) {
        messageText.text = message;
        opacity = 1;
        slideIn.restart();
    }

    function hide() {
        slideOut.restart();
    }

    SequentialAnimation {
        id: slideIn
        ParallelAnimation {
            NumberAnimation { target: notification; property: "opacity"; to: 1; duration: 300 }
            NumberAnimation { target: notification; property: "x"; to: parent.width - width - 20; duration: 300; easing.type: Easing.OutCubic }
        }
        PauseAnimation { duration: 3000 }
        ScriptAction { script: notification.hide() }
    }

    SequentialAnimation {
        id: slideOut
        ParallelAnimation {
            NumberAnimation { target: notification; property: "opacity"; to: 0; duration: 300 }
            NumberAnimation { target: notification; property: "x"; to: parent.width; duration: 300; easing.type: Easing.InCubic }
        }
    }
}
