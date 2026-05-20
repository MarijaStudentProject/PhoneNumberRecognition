import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: "#F8F8F8"

    property alias contactsModel: listView.model
    property string selectedName: ""
    property string selectedInitials: ""
    property string selectedPhone: ""
    property string selectedColor: "#E8B4B8"
    property bool showInfo: false
    property bool showDialing: false

    property int selectedIndex: -1
    property string selectedEmail: ""
    property string selectedAddress: ""

    signal callRequested(string name, string initials, string phone, string color)

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
                width: ListView.view.width
                height: visible ? 64 : 0
                color: "white"

                visible: searchField.text.length === 0 ||
                         model.name.toLowerCase().includes(searchField.text.toLowerCase())

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    spacing: 14

                    Rectangle {
                        width: 42; height: 42; radius: 21
                        color: model.color
                        Text {
                            anchors.centerIn: parent
                            text: model.initials
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            color: "white"
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text { text: model.name;  font.pixelSize: 15; color: "#1a1a1a" }
                        Text { text: model.phone; font.pixelSize: 13; color: "#888" }
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
                        contactInfo.contactIndex    = index
                        contactInfo.contactName     = model.name
                        contactInfo.contactInitials = model.initials
                        contactInfo.contactPhone    = model.phone
                        contactInfo.contactColor    = model.color
                        contactInfo.contactEmail    = model.email
                        contactInfo.contactAddress  = model.address
                        showInfo = true
                    }
                }
            }
        }
    }

    ContactInfo {
        id: contactInfo
        anchors.fill: parent
        visible: showInfo
        enabled: showInfo
        z: 10
        onDismissed:   showInfo = false
        contactName:     contactInfo.contactName
        contactInitials: contactInfo.contactInitials
        contactPhone:    contactInfo.contactPhone
        contactColor:    contactInfo.contactColor
        onCallClicked: {
            showDialing = false
            callRequested(contactInfo.contactName, contactInfo.contactInitials, contactInfo.contactPhone, contactInfo.contactColor)
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
