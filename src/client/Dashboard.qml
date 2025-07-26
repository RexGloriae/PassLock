import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Dialogs
import com.login 1.0

Item {
    id: dashboard
    width: parent.width
    height: parent.height

    Rectangle {
        width: parent.width
        height: parent.height
        color: "#222222"

        ColumnLayout {
            id: dashboard_view
            anchors.centerIn: parent
            spacing: 5

            // Display Image
            Image {
                source: "passlock.png"
                width: 150
                height: 150
                Layout.alignment: Qt.AlignHCenter
            }

            // Button for Account Settings
            Button {
                text: "Go to Account Settings"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    stackView_dash.push("AccountSettings.qml");  // Replace with your actual Account Settings page
                }
            }

            Text {
                text: loginManager.user
                font.pixelSize: 16
                font.bold: true
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }
        }

        // Sidebar
        Rectangle {
            id: sidebar
            width: 200
            height: parent.height
            color: "#333333"
            radius: 8
            anchors.left: parent.left

            ColumnLayout {
                spacing: 5
                anchors.fill: parent
                anchors.topMargin: 20

                Text {
                    text: "PassLock"
                    font.pixelSize: 24
                    font.bold: true
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    text: "Saved Passwords"
                    Layout.fillWidth: true
                    onClicked: {
                        stackView_dash.push("SavedPasswords.qml");
                    }
                }

                Button {
                    text: "Generate Password"
                    Layout.fillWidth: true
                    Layout.topMargin: -100
                    onClicked: {
                        stackView_dash.push("GeneratePassword.qml");
                    }
                }

                Button {
                    text: "Logout"
                    Layout.fillWidth: true
                    anchors.bottom: parent.bottom
                    onClicked: {
                        logoutDialog.open()
                    }
                }
            }
        }

        // StackView to switch between pages
        StackView {
            id: stackView_dash
            width: parent.width - sidebar.width
            height: parent.height
            anchors.right: parent.right
            initialItem: dashboard_view
        }

        // Logout Confirmation Dialog
        Popup {
            id: logoutDialog
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
                    text: "Are you sure you want to log out?"
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
                        onClicked: logoutDialog.close()
                    }

                    Button {
                        text: "Logout"
                        background: Rectangle { color: "red"; radius: 6 }
                        onClicked: {
                            stackView.pop();
                            logoutDialog.close();
                            loginManager.logout();
                        }
                    }
                }
            }
        }

    }
}
