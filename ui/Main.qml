import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 400
    height: 700
    minimumWidth: 360
    minimumHeight: 600
    title: "Contacts"

    property int currentTab: 0
    property string callingName: ""
    property string callingInitials: ""
    property string callingPhone: ""
    property string callingColor: "#E8B4B8"
    property bool showDialingFromSearch: false

    // Main content — fills the whole window
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#F8F8F8"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 8
                spacing: 8

                Rectangle {
                    Layout.fillWidth: true
                    height: 36
                    radius: 18
                    color: "transparent"

                    Rectangle {
                        anchors.fill: parent
                        radius: 18
                        color: currentTab === 0 ? "white" : "transparent"
                        border.color: currentTab === 0 ? "#E0E0E0" : "transparent"
                        border.width: 1
                    }

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 6
                        Text {
                            text: "\uf007"
                            font.family: fontAwesome.name
                            font.pixelSize: 14
                            color: currentTab === 0 ? "#1a1a1a" : "#888"
                        }
                        Text {
                            text: "Contacts"
                            font.pixelSize: 14
                            font.weight: currentTab === 0 ? Font.Medium : Font.Normal
                            color: currentTab === 0 ? "#1a1a1a" : "#888"
                        }
                    }
                    MouseArea { anchors.fill: parent; onClicked: currentTab = 0 }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 36
                    radius: 18
                    color: "transparent"

                    Rectangle {
                        anchors.fill: parent
                        radius: 18
                        color: currentTab === 1 ? "white" : "transparent"
                        border.color: currentTab === 1 ? "#E0E0E0" : "transparent"
                        border.width: 1
                    }

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 6
                        Text { text: "⠿"; font.pixelSize: 14 }
                        Text {
                            text: "Dial"
                            font.pixelSize: 14
                            font.weight: currentTab === 1 ? Font.Medium : Font.Normal
                            color: currentTab === 1 ? "#1a1a1a" : "#888"
                        }
                    }
                    MouseArea { anchors.fill: parent; onClicked: currentTab = 1 }
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: currentTab

            ContactsView {
                contactsModel: cppContactsModel
                onCallRequested: (name, initials, phone, color) => {
                    incomingCall.callLabel = "Calling..."
                    activeCall.show(name, initials, phone, color)
                }
            }
            DialView {
                    id: dialView
                    contactsModel: cppContactsModel
                    onContactCallRequested: (name, initials, phone, color) => {
                        callingName     = name
                        callingInitials = initials
                        callingPhone  = phone
                        callingColor    = color
                        activeCall.show(callingName, callingInitials, callingPhone, callingColor)
                        showDialingFromSearch = false
                    }
            }
        }
    }

    // Test button — floats over content, remove when wiring real signals
    Rectangle {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 16
        width: 110
        height: 34
        radius: 17
        color: "#1a1a1a"
        z: 10

        Text {
            anchors.centerIn: parent
            text: "Test Call"
            font.pixelSize: 13
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: incomingCall.show("Beatriz Souza", "BS", "(917) 555-0144", "#C4B4E8")
        }
    }

    // Incoming call popup — overlays everything
    IncomingCallPopup {
        id: incomingCall
        anchors.fill: parent
        z: 99
        onAccepted: {
            activeCall.show(incomingCall.contactName, incomingCall.contactInitials,
                            incomingCall.contactPhone, incomingCall.contactColor)
        }
        onDeclined: console.log("Call declined")
    }
    ActiveCallScreen {
        id: activeCall
        anchors.fill: parent
        z: 98
        onCallEnded: console.log("Call ended")
    }

    FontLoader {
        id: fontAwesome
        source: "qrc:PhoneApp/ui/fonts/fa-solid-900.ttf"
    }

}
