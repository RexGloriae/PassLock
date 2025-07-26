import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts
import QtQuick.Controls.Material
import com.login 1.0
import com.datachanger 1.0

Item {
    id: settingsPage
    width: parent.width
    height: parent.height

    DataChanger {
        id: dataChanger
        onEmailChangeResult: {
            emailField.text = ""
            confEmailField.text = ""
            if(res === 1) {
                notify.show("Email changed!");
            }
            else {
                notify.show("Email already used!\nTry Again!");
            }
        }
    }

    Notification{
        id: notify
    }

    ColumnLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        spacing: 60

        RowLayout {
            Button {
                text: "Change Email"
                onClicked: {
                    if (emailField.text === "") {
                        notify.show("You need to write\nan email address!");
                    }
                    else if (emailField.text != confEmailField.text) {
                        notify.show("The emails don't match!\nTry Again!");
                    }
                    else {
                        emailDialog.open();
                    }
                }
            }
            ColumnLayout {
                TextField {
                    id: emailField
                    placeholderText: "Enter new email..."
                    Layout.fillWidth: true
                }
                TextField {
                    id: confEmailField
                    placeholderText: "Confirm new email..."
                    Layout.fillWidth: true
                }
            }
        }
        RowLayout {
            Button {
                text: "Change Password"
                onClicked: {
                    if (is_alphnum_special(passField.text) === false) {
                        notify.show("Password can contain only alpha-numerical\ncharacters and allowed\nspecial characters!");
                    }
                    else if (passField.text === "") {
                        notify.show("Password field is empty!");
                    }
                    else if (passField.text.length < 8) {
                        notify.show("Passworld must be at least 8\ncharacters long.");
                    }
                    else if (passField.text !== confPassField.text) {
                        notify.show("Passwords do not match!\nTry Again!");
                    }
                    else {
                        passDialog.open();
                    }
                }
            }
            ColumnLayout {
                TextField {
                    id: passField
                    placeholderText: "Enter new password..."
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                }
                TextField {
                    id: confPassField
                    placeholderText: "Confirm new password..."
                    Layout.fillWidth: true
                    echoMode: TextInput.Password
                }
                Text{
                    text: "Allowed special characters: !@#$%&"
                    font.pixelSize: 8
                    font.bold: true
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
    }

    Item {
        Layout.fillHeight: true  // Pushes the buttons down
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
        id: emailDialog
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
                text: "Are you sure you want\nto change your email?"
                font.pixelSize: 16
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter

                Button {
                    text: "No"
                    background: Rectangle { color: "#474747"; radius: 6 }
                    onClicked: emailDialog.close()
                }

                Button {
                    text: "Yes"
                    background: Rectangle { color: "red"; radius: 6 }
                    onClicked: {
                        dataChanger.change_email(loginManager.user, emailField.text);
                        emailDialog.close();
                    }
                }
            }
        }
    }

    Popup {
        id: passDialog
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
                text: "Are you sure you want\nto change your password?"
                font.pixelSize: 16
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                spacing: 20
                Layout.alignment: Qt.AlignHCenter

                Button {
                    text: "No"
                    background: Rectangle { color: "#474747"; radius: 6 }
                    onClicked: passDialog.close()
                }

                Button {
                    text: "Yes"
                    background: Rectangle { color: "red"; radius: 6 }
                    onClicked: {
                        dataChanger.change_pass(loginManager.user, passField.text);
                        notify.show("Password changed!");
                        passDialog.close();
                        stackView_dash.pop();
                        stackView.pop();
                    }
                }
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
