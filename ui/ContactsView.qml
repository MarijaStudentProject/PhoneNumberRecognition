import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

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
            "#E8B4B8",
            "#B4C8E8",
            "#E8C4B4",
            "#C4B4E8",
            "#B4E8C8",
            "#E8E4B4",
            "#E8B4D4",
            "#B4D4E8",
            "#D4E8B4"
        ]

        return colors[Math.abs(id) % colors.length]
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
               Layout.topMargin: 8
               Layout.bottomMargin: 4
               height: 44
               color: "#F8F8F8"  // white background behind the pill
            Rectangle {
                anchors.fill: parent
                       anchors.leftMargin: 16
                       anchors.rightMargin: 16
                       radius: 22
                       color: "#EFEFEF"


                RowLayout {
                    anchors.fill: parent
                            anchors.leftMargin: 14
                            anchors.rightMargin: 14
                            spacing: 8

                    Text {
                        text: "\uf002"
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

                        onTextEdited: {
                            contactsModel.searchText = text
                        }
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
        }

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

           /*     visible: searchField.text.length === 0 ||
                         model.name.toLowerCase().includes(searchField.text.toLowerCase())
            */
                readonly property string displayName: fullName(name, surname)
                readonly property string displayInitials: initialsFrom(name, surname)
                readonly property string displayColor: colorForId(contactId)

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    spacing: 14

                    Rectangle {
                        width: 42; height: 42; radius: 21
                        color: displayColor
                        Text {
                            anchors.centerIn: parent
                            text: displayInitials
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            color: "white"
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        spacing: 2
                        Text { text: displayName;  font.pixelSize: 15; color: "#1a1a1a"; horizontalAlignment: Text.AlignLeft}
                        Text { text: phone; font.pixelSize: 13; color: "#888"; horizontalAlignment: Text.AlignLeft }
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

    ContactInfo {
        id: contactInfo
        anchors.fill: parent
        visible: showInfo
        contactName:     selectedName
        contactInitials: selectedInitials
        contactPhone:    selectedPhone
        contactEmail:   selectedEmail
        contactColor:    selectedColor
        onDismissed:   showInfo = false
        // contactName:     contactInfo.contactName
        // contactInitials: contactInfo.contactInitials
        // contactPhone:    contactInfo.contactPhone
        // contactColor:    contactInfo.contactColor
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
