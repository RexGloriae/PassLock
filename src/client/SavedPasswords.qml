import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts
import QtQuick.Controls.Material
import com.passget 1.0
import com.login 1.0
import com.clipboard 1.0

Item {
    width: parent.width
    height: parent.height

    Notification{
        id: notify
    }

    PassGetManager{
        id: passGetManager
    }

    ClipboardHelper{
        id: clipboardHelper
    }

    property bool showPasswords: false
    property string currentPassword: ""

    ListModel {
        id: passwordModel
    }

    function fetchPasswords() {
        passwordModel.clear()
        const list = passGetManager.get(loginManager.user)
        for (let i = 0; i < list.length; i++) {
            console.log("ADDED:", list[i]);
            passwordModel.append({
                                     platform: list[i]["platform"],
                                     username: list[i]["username"],
                                     password: list[i]["password"]
                                 });
        }
    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Text {
            text: "Saved Passwords"
            font.pixelSize: 24
            color: "white"
        }

        Button {
            text: showPasswords ? "Hide My Passwords" : "Show My Passwords"
            onClicked: {
                showPasswords = !showPasswords
                if (showPasswords) fetchPasswords();
            }
        }

        ListView {
            visible: showPasswords
            Layout.fillWidth: true
            height: 300
            clip: true
            model: passwordModel

            delegate: Rectangle {
                color: "transparent"
                radius: 6
                anchors.left: parent.left
                anchors.right: parent.right
                height: 150
                border.color: "#444"

                property bool showPasswordField: false

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    GridLayout {
                        columns: 3
                        columnSpacing: 30
                        rowSpacing: 10

                        Text {
                            text: "Platform:\n" + model.platform
                            font.pixelSize: 18
                            color: "gray"
                            wrapMode: Text.Wrap
                            Layout.preferredWidth: 150
                        }

                        Text {
                            text: "Username:\n" + model.username
                            font.pixelSize: 18
                            color: "white"
                            wrapMode: Text.Wrap
                            Layout.preferredWidth: 150
                        }

                        Text {
                            text: "Password:\n" + (showPasswordField ? model.password : "••••••••")
                            font.pixelSize: 18
                            color: "white"
                            wrapMode: Text.WrapAnywhere
                            Layout.preferredWidth: 150
                        }
                    }

                    GridLayout {
                        columns: 3
                        columnSpacing: 100
                        rowSpacing: 10

                        Button {
                            text: "Copy"
                            background: Rectangle { color: "gray"; radius: 4 }
                            onClicked: {
                                clipboardHelper.copyText(model.password)
                                notify.show("Password copied\nto clipboard!");
                            }
                        }

                        Button {
                            text: "Delete"
                            background: Rectangle { color: "red"; radius: 4 }
                            onClicked: {
                                currentPassword = model.password;
                                deleteDialog.open();
                            }
                        }

                        Button {
                            text: showPasswordField ? "Hide" : "Show"
                            onClicked: showPasswordField = !showPasswordField
                        }
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }

        Button {
            text: "Back"
            Layout.alignment: Qt.AlignHCenter
            onClicked: stackView_dash.pop()
        }

        Popup {
            id: deleteDialog
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
                    text: "Are you sure you want\nto delete this password?"
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
                        onClicked: deleteDialog.close()
                    }

                    Button {
                        text: "Yes"
                        background: Rectangle { color: "red"; radius: 6 }
                        onClicked: {
                            deleteDialog.close();
                            passGetManager.del(loginManager.user, currentPassword);
                            showPasswords = false;
                            fetchPasswords();
                            showPasswords = true;
                            notify.show("Password deleted!");
                        }
                    }
                }
            }
        }
    }
}
