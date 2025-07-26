import QtQuick 2.15
import QtQuick.Controls 2.15
import com.login 1.0
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Dialogs

Item {
    visible: true

    Notification {
        id: notify
    }

    property string username: ""

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

            // Password input field
            TextField {
                id: passwordField
                placeholderText: "Password"
                width: 400
                font.pixelSize: 18
                color: "white"
                echoMode: TextInput.Password

                background: Rectangle {
                    color: "#474040"
                    radius: 8
                    border.color: "#555555"
                }

                onFocusChanged: {
                    if (focus) {
                        passwordPopup.open();
                    } else {
                        passwordPopup.close();
                    }
                }
            }

            // Confirm Password input field
            TextField {
                id: confirmPasswordField
                placeholderText: "Confirm Password"
                width: 400
                font.pixelSize: 18
                color: "white"
                echoMode: TextInput.Password

                background: Rectangle {
                    color: "#474040"
                    radius: 8
                    border.color: "#555555"
                }
            }

            Row {
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    contentItem: Text {
                        text: "Reset Password"
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
                        if (is_alphnum_special(passwordField.text) === false) {
                            notify.show("Password can contain only alpha-numerical\ncharacters and allowed\nspecial characters!");
                        }
                        else if (passwordField.text === "") {
                            notify.show("Password field is empty!");
                        }
                        else if (passwordField.text.length < 8) {
                            notify.show("Passworld must be at least 8\ncharacters long.");
                        }
                        else if (passwordField.text !== confirmPasswordField.text) {
                            notify.show("Passwords do not match!\nPlease try again!");
                        }
                        else {
                            passwordReset.change_pass(username, passwordField.text);
                            notify.show("Password reseted!");
                            stackView.pop();
                            stackView.pop();
                            passwordField.text = ""
                            confirmPasswordField.text = ""
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
                        stackView.pop();
                        passwordField.text = ""
                        confirmPasswordField.text = ""
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

    Popup {
        id: passwordPopup
        width: 350
        height: 80
        modal: false
        closePolicy: Popup.CloseOnEscape
        y: passwordField.y  // Position it above the password field
        x: passwordField.x //+ (passwordField.width / 2) - (width / 2)  // Center it horizontally
        background: Rectangle {
            color: "#333333"
            radius: 8
            border.color: "#555555"
            border.width: 1
        }

        Column {
            anchors.centerIn: parent
            spacing: 5

            Text {
                text: "Password must contain:"
                font.pixelSize: 16
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "- At least 8 characters"
                font.pixelSize: 14
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "- Alphanumeric and special characters (!@#$%&)"
                font.pixelSize: 14
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
    function is_alphnum(inputText) {
        return /^[a-zA-Z0-9]+$/.test(inputText);
    }
    function is_alphnum_special(input) {
        var regex = /^[a-zA-Z0-9!@#$%&]*$/;
        return regex.test(input);
    }
}
