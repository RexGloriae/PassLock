import QtQuick 2.15
import QtQuick.Controls 2.15
import com.login 1.0
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Dialogs

Item {
    visible: true

    LoginManager {
        id: loginManager
        onLoginResult: {
            if (res === 1) {
                usernameField.text = "";
                passwordField.text = "";
                stackView.push(Qt.createComponent("Dashboard.qml"));
            } else {
                notify.show("Login failed. Try Again!");
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
                placeholderText: "Username"
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

            // Password input field
            TextField {
                id: passwordField
                placeholderText: "Password"
                width: 400
                font.pixelSize: 18
                color: "white"  // Text color
                echoMode: TextInput.Password

                background: Rectangle {
                    color: "#474040"  // Dark background color
                    radius: 8  // Rounded corners
                    border.color: "#555555"  // Subtle border color
                }
            }

            Button {
                background: Rectangle {
                    color: "transparent"
                }
                width: 400
                height: 20
                padding: 0

                contentItem: Text {
                    text: "Forgot my password"
                    font.pixelSize: 14
                    color: "#AAAAAA"
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    anchors.right: parent.right
                }

                onClicked: {
                    stackView.push("ForgotPassword.qml");
                }
            }

            // Login button
            Row {
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    width: 100

                    contentItem: Text {
                        text: "Login"
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
                            notify.show("Username field is empty!");
                        }
                        else if (passwordField.text === "") {
                            notify.show("Password field is empty!");
                        }
                        else {
                            loginManager.login(usernameField.text, passwordField.text);
                        }
                    }
                }

                Button {
                    contentItem: Text {
                        text: "Create a Profile"
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
                        // Create a new instance of the Register component dynamically
                        stackView.push("Register.qml");
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
