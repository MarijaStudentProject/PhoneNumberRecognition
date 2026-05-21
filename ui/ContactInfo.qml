import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#80000000"
    visible: false

    property int    contactIndex: -1
    property string contactName: ""
    property string contactSurname: ""
    property string contactInitials: ""
    property string contactPhone: ""
    property string contactColor: "#E8B4B8"
    property string contactEmail: ""
    property string contactAddress: ""

    property bool editMode: false

    signal dismissed()
    signal callClicked(string phone)

    onVisibleChanged: {
        if (!visible) {
            editMode = false
        } else {
            Qt.callLater(function() {
                nameField.text    = contactName
                phoneField.text   = contactPhone
                emailField.text   = contactEmail
                addressField.text = contactAddress
            })
        }
    }

    onContactNameChanged:    nameField.text    = contactName
    onContactPhoneChanged:   phoneField.text   = contactPhone
    onContactEmailChanged:   emailField.text   = contactEmail
    onContactAddressChanged: addressField.text = contactAddress

    MouseArea {
        anchors.fill: parent
        enabled: root.visible
        onClicked: {root.dismissed(); clickSound.play()}
    }

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - 48
        height: cardContent.implicitHeight + 48
        radius: 16
        color: "white"

        MouseArea { anchors.fill: parent; onClicked: clickSound.play()}

        // Close
        Text {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 12
            anchors.rightMargin: 14
            text: "\uf00d"
            font.pixelSize: 14
            color: "#888"
            MouseArea {
                anchors.fill: parent
                onClicked: { editMode = false; root.dismissed(); clickSound.play() }
            }
        }

        // Edit / Save
        Text {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 12
            anchors.leftMargin: 14
            text: editMode ? "Save" : "Edit"
            font.pixelSize: 14
            color: editMode ? "#00897B" : "#888"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (editMode) {
                        detailesContactModel.name   = nameField.text
                        detailesContactModel.surname = surnameField.text
                        detailesContactModel.email  = emailField.text
                        detailesContactModel.street = addressField.text
                        detailesContactModel.setPhoneNumber(0, phoneField.text)
                        detailesContactModel.save()

                        root.contactName    = nameField.text
                        root.contactPhone   = phoneField.text
                        root.contactEmail   = emailField.text
                        root.contactAddress = addressField.text

                    }
                    clickSound.play()
                    editMode = !editMode
                }
            }
        }

        ColumnLayout {
            id: cardContent
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 24
            spacing: 0

            // Avatar
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 24
                width: 72; height: 72; radius: 36
                color: root.contactColor
                Text {
                    anchors.centerIn: parent
                    text: root.contactInitials
                    font.pixelSize: 24
                    font.weight: Font.Medium
                    color: "white"
                }
            }

            // Name
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 12
                Layout.bottomMargin: 16
                spacing: 6

                TextField {
                    id: nameField
                    text: root.contactName
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    color: "#1a1a1a"
                    readOnly: !editMode
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle {
                        color: editMode ? "#F9F9F9" : "transparent"
                        border.color: editMode ? "#E0E0E0" : "transparent"
                        radius: 6
                    }
                    onTextEdited: keyboardSound.play()

                }

                TextField {
                    id: surnameField
                    text: root.contactSurname
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    color: "#1a1a1a"
                    readOnly: !editMode
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle {
                        color: editMode ? "#F9F9F9" : "transparent"
                        border.color: editMode ? "#E0E0E0" : "transparent"
                        radius: 6
                    }
                    onTextEdited: keyboardSound.play()

                }
            }
            // Call + favourite (hidden in edit mode)
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 20
                spacing: 12
                visible: !editMode

                Rectangle {
                    width: 90; height: 34; radius: 17
                    color: "#00897B"
                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 6
                        Text { text: "\uf095"; font.pixelSize: 13 }
                        Text { text: "Call"; font.pixelSize: 14; font.weight: Font.Medium; color: "white" }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {root.callClicked(root.contactPhone); clickSound.play()}
                    }
                }

                Rectangle {
                    width: 34; height: 34; radius: 17
                    color: "#F2F2F2"
                    property bool isFavourite: false
                    Text { anchors.centerIn: parent;
                        text: parent.isFavourite ? "\uf005" : "\uf006"
                        font.pixelSize: 16;
                        font.family: fontAwesome.name
                        color: parent.isFavourite ? "#FFD700" : "#888"
                    }
                    MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                parent.isFavourite = !parent.isFavourite
                                clickSound.play()
                            }
                        }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#F0F0F0"
                Layout.bottomMargin: 16
            }

            // Phone
            ColumnLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 14
                spacing: 2
                Text { text: "PHONE"; font.pixelSize: 11; color: "#888" }
                TextField {
                    id: phoneField
                    Layout.fillWidth: true
                    text: root.contactPhone
                    font.pixelSize: 14
                    color: "#1a1a1a"
                    readOnly: !editMode
                    background: Rectangle {
                        color: editMode ? "#F9F9F9" : "transparent"
                        border.color: editMode ? "#E0E0E0" : "transparent"
                        radius: 6
                    }
                    onTextEdited: keyboardSound.play()
                }
            }

            // Email
            ColumnLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 14
                spacing: 2
                visible: root.contactEmail.length > 0 || editMode
                Text { text: "EMAIL"; font.pixelSize: 11; color: "#888" }
                TextField {
                    id: emailField
                    Layout.fillWidth: true
                    text: root.contactEmail
                    font.pixelSize: 14
                    color: "#1a1a1a"
                    readOnly: !editMode
                    background: Rectangle {
                        color: editMode ? "#F9F9F9" : "transparent"
                        border.color: editMode ? "#E0E0E0" : "transparent"
                        radius: 6
                    }
                    onTextEdited: keyboardSound.play()

                }
            }

            // Address
            ColumnLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 24
                spacing: 2
                visible: root.contactAddress.length > 0 || editMode
                Text { text: "ADDRESS"; font.pixelSize: 11; color: "#888" }
                TextField {
                    id: addressField
                    Layout.fillWidth: true
                    text: root.contactAddress
                    font.pixelSize: 14
                    color: "#1a1a1a"
                    readOnly: !editMode
                    background: Rectangle {
                        color: editMode ? "#F9F9F9" : "transparent"
                        border.color: editMode ? "#E0E0E0" : "transparent"
                        radius: 6
                    }
                    onTextEdited: keyboardSound.play()

                }
            }
        }
    }
}
