import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts
import QtQuick.Controls.Material
import com.passgen 1.0
import com.login 1.0
import com.clipboard 1.0

Item {
    id: generatorPage
    property string generatedPassword: ""
    width: parent.width
    height: parent.height

    Notification{
        id: notify
    }

    ClipboardHelper{
        id: clipboardHelper
    }

    PassGenManager{
        id: passGenManager
        onGeneratedPass: handleGeneratedPassword(pass)
    }

    function handleGeneratedPassword(pass) {
        usernameField.text = "";
        platformField.text = "";
        lengthField.text = "";
        specialCharsCheck.checkState = false;
        capitalLettersCheck.checkState = false;
        nonCapitalLettersCheck.checkState = false;
        digitsCheck.checkState = false;
        if (pass === "Collision"){
            notify.show("You already have\na password for this\nplatform and username!");
        }
        else {
            notify.show("Password generated successfully!");
            generatedPassword = pass;
            keepGeneratedDialog.open();
        }
    }

    ColumnLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        spacing: 20

        Text {
            text: "Generate Password"
            font.pixelSize: 24
            color: "white"
        }

        RowLayout {
            spacing: 10

            Text {
                text: "Password Length:"
                color: "white"
                Layout.preferredWidth: 150  // Ensures enough space
                Layout.alignment: Qt.AlignLeft
            }

            TextField {
                id: lengthField
                placeholderText: "Enter length..."
                validator: IntValidator { bottom: 8; top: 32 }
                Layout.fillWidth: true
                Layout.maximumWidth: 120
            }
        }

        ColumnLayout{
            RowLayout {
                spacing: 10

                CheckBox {
                    id: specialCharsCheck
                    text: "Include special characters"
                    width: 150
                    Layout.alignment: Qt.AlignLeft
                    checked: false
                }

                CheckBox {
                    id: digitsCheck
                    text: "Include digits"
                    width: 150
                    Layout.alignment: Qt.AlignLeft
                    checked: false
                }
            }

            RowLayout{
                CheckBox {
                    id: capitalLettersCheck
                    text: "Include upper-case letters"
                    width: 150
                    Layout.alignment: Qt.AlignLeft
                    checked: false
                }

                CheckBox {
                    id: nonCapitalLettersCheck
                    text: "Include lower-case letters"
                    width: 150
                    Layout.alignment: Qt.AlignLeft
                    checked: false
                }
            }
        }

        RowLayout {
            spacing: 10

            Text {
                text: "Platform to use password on:"
                color: "white"
                Layout.preferredWidth: 150
                Layout.alignment: Qt.AlignLeft
            }

            TextField {
                id: platformField
                placeholderText: "Enter platform..."
                Layout.fillWidth: true
            }
        }

        // New field for "Username to associate password with"
        RowLayout {
            spacing: 10

            Text {
                text: "Username to associate\npassword with:"
                color: "white"
                Layout.preferredWidth: 150
                Layout.alignment: Qt.AlignLeft
            }

            TextField {
                id: usernameField
                placeholderText: "Enter username..."
                Layout.fillWidth: true
            }
        }

        Button {
            text: "Generate"
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                let length = parseInt(lengthField.text);
                let platform_len = parseInt(platformField.text);
                let username_len = parseInt(usernameField.text);
                if (isNaN(length) || length < 8 || length > 32) {
                    console.log("Invalid password length");
                    notify.show("Password length should be\nbetween 8 and 32.")
                }
                else if (usernameField.text === "") {
                    notify.show("You need to put in a\nusername to associate the\npassword with!");
                }
                else if (platformField.text === "") {
                    notify.show("You need to put in a\nplatform to associate the\npassword with!");
                }
                else if (platform_len > 32) {
                    notify.show("Platform must be less\nthan 32 characters!");
                }
                else if (username_len > 32) {
                    notify.show("Username must be less\nthan 32 characters!");
                }
                else if (!(specialCharsCheck.checked
                           || capitalLettersCheck.checked
                           || nonCapitalLettersCheck.checked
                           || digitsCheck.checked)) {
                    notify.show("You need to check\nat least one criteria!");
                }
                else {
                    passGenManager.gen(length,
                                       specialCharsCheck.checked,
                                       digitsCheck.checked,
                                       capitalLettersCheck.checked,
                                       nonCapitalLettersCheck.checked,
                                       loginManager.user,
                                       usernameField.text,
                                       platformField.text);
                }
            }
        }

        Item {
            Layout.fillHeight: true  // Pushes the buttons down
        }
    }


    Button {
        text: "Back"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        onClicked: {
            stackView_dash.pop()
        }
    }

    Popup {
        id: keepGeneratedDialog
        width: 300
        height: 150
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        x: (parent.width - width) / 2 - 100
        y: (parent.height - height) / 2
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
                text: "This password was generated for you:"
                font.pixelSize: 16
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: generatedPassword
                font.pixelSize: 16
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter

                Button {
                    text: "Okay, thanks!"
                    background: Rectangle { color: "#474747"; radius: 6 }
                    onClicked: keepGeneratedDialog.close()
                }

                Button {
                    text: "Copy password"
                    background: Rectangle { color: "#474747"; radius: 6 }
                    onClicked: {
                        keepGeneratedDialog.close();
                        clipboardHelper.copyText(generatedPassword);
                        notify.show("Password copied\nto clipboard!");
                    }
                }
            }
        }
    }

}
