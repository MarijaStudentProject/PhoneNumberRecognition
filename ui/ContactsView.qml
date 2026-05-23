
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

Rectangle {
    color: "#F8F8F8"

    property var contactsModel: null
    property string selectedName: ""
    property string selectedInitials: ""
    property string selectedPhone: ""
    property string selectedEmail: ""
    property string selectedColor: "#E8B4B8"
    property bool showInfo: false
    property bool showDialing: false

    signal callRequested(string name, string initials, string phone, string color)

    function fullName(name, surname){
        if(surname === undefined || surname.length === 0)
            return name
        return name + " " + surname
    }

    function initialsFrom(name, surname){
        var first = name && name.length > 0 ? name[0] : ""
        var second = surname && surname.length > 0 ? surname[0] : ""
        if(second.length === 0 && name.indexOf(" ") !== -1){
            var parts = name.split(" ")
            second = parts.length > 1 && parts[1].length > 0 ? parts[1][0] : ""
        }
        return (first + second).toUpperCase()
    }

    function colorForId(id) {
        var colors = [
            "#E8B4B8", "#B4C8E8", "#E8C4B4", "#C4B4E8",
            "#B4E8C8", "#E8E4B4", "#E8B4D4", "#B4D4E8", "#D4E8B4"
        ]
        return colors[Math.abs(id) % colors.length]
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Search bar
        Rectangle {
            Layout.fillWidth: true
            Layout.topMargin: 8
            Layout.bottomMargin: 4
            height: 44
            color: "#F8F8F8"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 8
                spacing: 8

                Rectangle {
                    Layout.fillWidth: true
                    height: 44
                    radius: 22
                    color: "#EFEFEF"

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 14
                        anchors.rightMargin: 14
                        spacing: 8

                        Text {
                            text: ""
                            font.family: fontAwesome.name
                            font.pixelSize: 14
                            color: "#888"
                        }

                        TextInput {
                            id: searchField
                            Layout.fillWidth: true
                            font.pixelSize: 15
                            color: "#333"
                            clip: true
                            text: contactsModel ? contactsModel.searchText : ""
                            onTextEdited: { contactsModel.searchText = text; keyboardSound.play() }

                            Text {
                                anchors.fill: parent
                                text: "Search contacts"
                                color: "#AAAAAA"
                                font.pixelSize: 15
                                verticalAlignment: Text.AlignVCenter
                                visible: searchField.text.length === 0
                            }
                        }
                    }
                }

                Item {
                    width: 32
                    height: 44

                    Text {
                        anchors.centerIn: parent
                        text: ""
                        font.family: fontAwesome.name
                        font.pixelSize: 18
                        color: "#555"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: kebabMenu.popup()
                    }

                    Menu {
                        id: kebabMenu
                        topPadding: 6
                        bottomPadding: 6

                        background: Rectangle {
                            implicitWidth: 180
                            radius: 12
                            color: "white"
                            border.color: "#E5E5E5"
                            border.width: 1
                        }

                        MenuItem {
                            id: importMenuItem
                            height: 44
                            topPadding: 0
                            bottomPadding: 0
                            leftPadding: 0
                            rightPadding: 0

                            contentItem: Row {
                                anchors.fill: parent
                                anchors.leftMargin: 16
                                anchors.rightMargin: 16
                                spacing: 10

                                Text {
                                    height: parent.height
                                    text: ""
                                    font.family: fontAwesome.name
                                    font.pixelSize: 14
                                    color: "#555"
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    height: parent.height
                                    text: "Import contacts"
                                    font.pixelSize: 15
                                    color: "#1a1a1a"
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }

                            background: Rectangle {
                                radius: 8
                                anchors.margins: 4
                                color: importMenuItem.highlighted ? "#F0F0F0" : "white"
                            }

                            onTriggered: fileDialog.open()
                        }

                        MenuItem {
                            id: addContactMenuItem
                            height: 44
                            topPadding: 0
                            bottomPadding: 0
                            leftPadding: 0
                            rightPadding: 0

                            contentItem: Row {
                                anchors.fill: parent
                                anchors.leftMargin: 16
                                anchors.rightMargin: 16
                                spacing: 10

                                Text {
                                    height: parent.height
                                    text: "\uf234"
                                    font.family: fontAwesome.name
                                    font.pixelSize: 14
                                    color: "#555"
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    height: parent.height
                                    text: "Add contact"
                                    font.pixelSize: 15
                                    color: "#1a1a1a"
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }

                            background: Rectangle {
                                radius: 8
                                anchors.margins: 4
                                color: addContactMenuItem.highlighted ? "#F0F0F0" : "white"
                            }

                            onTriggered: {
                                detailesContactModel.prepareNew()
                                contactInfo.contactIndex    = -1
                                contactInfo.contactName     = ""
                                contactInfo.contactSurname  = ""
                                contactInfo.contactInitials = ""
                                contactInfo.contactPhone    = ""
                                contactInfo.contactEmail    = ""
                                contactInfo.contactAddress  = ""
                                contactInfo.contactColor    = "#B4C8E8"
                                contactInfo.editMode        = true
                                showInfo = true
                                clickSound.play()
                            }
                        }
                    }
                }
            }
        }

        // Contacts list
        ListView {
            id: listView
            model: contactsModel
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            section.property: "name"
            section.criteria: ViewSection.FirstCharacter
            section.delegate: Rectangle {
                width: ListView.view.width
                height: 28
                color: "#F8F8F8"
                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    anchors.verticalCenter: parent.verticalCenter
                    text: section
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: "#555"
                }
            }

            delegate: Rectangle {
                required property int index
                required property int contactId
                required property string name
                required property string surname
                required property string phone

                width: ListView.view.width
                height: visible ? 64 : 0
                color: "white"

                readonly property string displayName:     fullName(name, surname)
                readonly property string displayInitials: initialsFrom(name, surname)
                readonly property string displayColor:    colorForId(contactId)

                Row {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    spacing: 14

                    // Avatar
                    Rectangle {
                        width: 42; height: 42
                        radius: 21
                        anchors.verticalCenter: parent.verticalCenter
                        color: displayColor

                        Text {
                            anchors.centerIn: parent
                            text: displayInitials
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            color: "white"
                        }
                    }

                    // Name + phone
                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2

                        Text {
                            text: displayName
                            font.pixelSize: 15
                            color: "#1a1a1a"
                        }
                        Text {
                            text: phone
                            font.pixelSize: 13
                            color: "#888"
                        }
                    }
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 72
                    anchors.right: parent.right
                    height: 1
                    color: "#F0F0F0"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        contactsModel.select(index)
                        contactInfo.contactIndex    = index
                        contactInfo.contactName     = name
                        contactInfo.contactSurname  = surname
                        contactInfo.contactInitials = displayInitials
                        contactInfo.contactPhone    = phone
                        contactInfo.contactEmail    = detailesContactModel.email
                        contactInfo.contactAddress  = detailesContactModel.street
                        contactInfo.contactColor    = displayColor
                        showInfo = true
                        clickSound.play()
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Import VCF file"
        nameFilters: ["VCard files (*.vcf)", "All files (*)"]
        onAccepted: {
            var path = fileDialog.selectedFile.toLocalFile()
            contactsModel.import(path)
        }
    }

    ContactInfo {
        id: contactInfo
        anchors.fill: parent
        visible: showInfo
        contactName:     selectedName
        contactInitials: selectedInitials
        contactPhone:    selectedPhone
        contactEmail:    selectedEmail
        contactColor:    selectedColor
        onDismissed:   showInfo = false
        onCallClicked: {
            showDialing = false
            callRequested(contactName, contactInitials, contactPhone, contactColor)
        }
    }

    DialingPopup {
        anchors.fill: parent
        visible: showDialing
        enabled: showDialing
        z: 10
        contactName:     contactInfo.contactName
        contactInitials: contactInfo.contactInitials
        contactPhone:    contactInfo.contactPhone
        contactColor:    contactInfo.contactColor
        onDismissed: showDialing = false
        onCallClicked: {
            showDialing = false
            callRequested(contactInfo.contactName, contactInfo.contactInitials, contactInfo.contactPhone, contactInfo.contactColor)
        }
    }
}
