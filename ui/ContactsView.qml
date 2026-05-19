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

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 48
            color: "white"
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            Layout.topMargin: 8
            Layout.bottomMargin: 4

            Rectangle {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                radius: 20
                color: "#F2F2F2"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    spacing: 8

                    Text { text: "🔍"; font.pixelSize: 14; color: "#888" }

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
                        selectedName     = model.name
                        selectedInitials = model.initials
                        selectedPhone    = model.phone
                        selectedColor    = model.color
                        showInfo = true
                    }
                }
            }
        }
    }

    ContactInfo {
        anchors.fill: parent
        visible: showInfo
        contactName:     selectedName
        contactInitials: selectedInitials
        contactPhone:    selectedPhone
        contactColor:    selectedColor
        onDismissed:   showInfo = false
        onCallClicked: { showInfo = false; showDialing = true }
    }

    DialingPopup {
        anchors.fill: parent
        visible: showDialing
        contactName:     selectedName
        contactInitials: selectedInitials
        contactPhone:    selectedPhone
        contactColor:    selectedColor
        onDismissed: showDialing = false
        onCallClicked: { showInfo = false; showDialing = true }
    }
}
