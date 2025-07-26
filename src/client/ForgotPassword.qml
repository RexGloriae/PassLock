import QtQuick 2.15
import QtQuick.Controls 2.15
import com.login 1.0
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Dialogs
import com.passreset 1.0

Item {
    visible: true

    PasswordReset{
        id: passwordReset
        onCodeSent: {
            if(res === 1) {
                notify.show("Code has been sent!");
            }
            else notify.show("Username is not registred!\nTry Again!");
        }
        onCodeOkay: {
            if (res === 0) {
                notify.show("The code you've\nentered is wrong!\nTry Again!");
            }
            else {
                codeField.text = ""
                codeField.text = ""

                let component = Qt.createComponent("PasswordReset.qml");
                if (component.status === Component.Ready) {
                    stackView.push(component, {username: usernameField.text });
                    usernameField.text = "";
                }
            }
        }
    }

    Notification {
        id: notify
    }

    Column {
        // This ensures that the image and text are placed at the top
        width: parent.width
        height: parent.height // Make the column take up all the space
        spacing: 30

        // Image
        Image {
            source: "passlock.png"
            width: 150
            height: 150
            fillMode: Image.PreserveAspectCrop
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top // Anchors the image below the text
            anchors.topMargin: 40 // Adds a margin between the text and the image
            z: 1
        }


        // Spacer to push login form down below
        Item {
            height: 20
        }
    }
    // Login form container
    Rectangle {
        width: 450
        height: 250
        color:"#222222"
        radius: 10
        border.color: "#444444"
        border.width: 2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 50

        Column {
            anchors.centerIn: parent
            spacing: 20

            // Username input field
            TextField {
                id: usernameField
                placeholderText: "Enter username"
                width: 400
                font.pixelSize: 18
                color: "white"  // Text color

                background: Rectangle {
                    color: "#474040"  // Dark background color
                    radius: 8  // Rounded corners
                    border.color: "#555555"  // Subtle border color
                }
            }

            onFocusChanged: {
                if (usernameField.text !== "") {
                    usernameField.placeholderText = "";
                }
            }

            TextField {
                id: codeField
                placeholderText: "Enter code"
                width: 400
                font.pixelSize: 18
                validator: IntValidator { bottom: 1000; top: 9999 }
                color: "white"  // Text color

                background: Rectangle {
                    color: "#474040"  // Dark background color
                    radius: 8  // Rounded corners
                    border.color: "#555555"  // Subtle border color
                }
            }

            Row {
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    width: 100

                    contentItem: Text {
                        text: "Send code"
                        font.pixelSize: 18
                        color: "white"  // Change the font color here
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    background: Rectangle {
                        color: "#474040"  // Background color of the button
                        radius: 8
                    }

                    onClicked: {
                        if (usernameField.text === "") {
                            notify.show("You need to enter\nan username!");
                        }
                        else {
                            passwordReset.send_recovery_code_on_email(usernameField.text);
                        }
                    }
                }

                Button {
                    contentItem: Text {
                        text: "Reset Password"
                        font.pixelSize: 18
                        color: "white"  // Change the font color here
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    background: Rectangle {
                        color: "#474040"  // Background color of the button
                        radius: 8
                    }
                    onClicked: {
                        let length = parseInt(codeField.text);
                        if (isNaN(length) || length < 1000 || length > 9999) {
                            notify.show("Password reset code is\nof 4 digits!")
                        }
                        else if(usernameField.text === "") {
                            notify.show("You need to enter\nan username!");
                        }
                        else if (codeField.text === "") {
                            notify.show("You need to enter\nthe received code!");
                        }
                        else {
                            passwordReset.check_recovery_code(usernameField.text, codeField.text);
                        }
                    }
                }

                Button {
                    contentItem: Text {
                        text: "Back"
                        font.pixelSize: 18
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    background: Rectangle {
                        color: "#474040"
                        radius: 8
                    }

                    onClicked: {
                        stackView.pop();
                        usernameField.text = ""
                        codeField.text = ""
                    }
                }
            }
        }
        // Feedback text
        Text {
            id: feedbackText
            color: "red"
            font.pixelSize: 16
            visible: false
        }
    }

    Button {
        text: "Quit"
        Layout.fillWidth: true
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        onClicked: {
            quitDialog.open()
        }
    }

    Popup {
        id: quitDialog
        width: 300
        height: 150
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        anchors.centerIn: parent
        background: Rectangle {
            color: "#333333"
            radius: 8
            border.color: "#555555"
            border.width: 1
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 10

            Text {
                text: "Are you sure you want to quit?"
                font.pixelSize: 16
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter

                Button {
                    text: "Cancel"
                    background: Rectangle { color: "#474747"; radius: 6 }
                    onClicked: quitDialog.close()
                }

                Button {
                    text: "Quit"
                    background: Rectangle { color: "red"; radius: 6 }
                    onClicked: {
                        quitDialog.close();
                        Qt.quit();
                    }
                }
            }
        }
    }
}
