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

    ListModel {
        id: contactsModel
        ListElement { initials: "AK"; name: "Aanya Krishnan"; phone: "(415) 555-0182"; color: "#E8B4B8" }
        ListElement { initials: "AW"; name: "Adam Werner";    phone: "(212) 555-0193"; color: "#B4C8E8" }
        ListElement { initials: "AP"; name: "Alicia Park";    phone: "+44 20 7946 0921"; color: "#E8C4B4" }
        ListElement { initials: "BS"; name: "Beatriz Souza";  phone: "(917) 555-0144"; color: "#C4B4E8" }
        ListElement { initials: "CH"; name: "Caleb Hsu";      phone: "(646) 555-0117"; color: "#B4E8C8" }
        ListElement { initials: "CR"; name: "Camille Roux";   phone: "+33 1 70 36 92 47"; color: "#E8E4B4" }
        ListElement { initials: "DO"; name: "Daniela Ortega"; phone: "(312) 555-0186"; color: "#E8B4D4" }
        ListElement { initials: "EB"; name: "Eli Bergman";    phone: "(503) 555-0162"; color: "#B4D4E8" }
        ListElement { initials: "FK"; name: "Faye Kobayashi"; phone: "(206) 555-0129"; color: "#D4E8B4" }
        ListElement { initials: "ŽK"; name: "Žaye Kobayashi"; phone: "(206) 555-0129"; color: "#D4E8B4" }

    }

    // Main content — fills the whole window
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "white"

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
                        Text { text: "👤"; font.pixelSize: 14 }
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
                contactsModel: contactsModel
                onCallRequested: (name, initials, phone, color) => {
                    incomingCall.callLabel = "Calling..."
                    incomingCall.show(name, initials, phone, color)
                }
            }
            DialView {
                    id: dialView
                    contactsModel: contactsModel
                    onContactCallRequested: (name, initials, phone, color) => {
                        callingName     = name
                        callingInitials = initials
                        callingPhone    = phone
                        callingColor    = color
                        showDialingFromSearch = true
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
        focus: true
        z: 99
        onAccepted: console.log("Call accepted")
        onDeclined: console.log("Call declined")
    }

    DialingPopup {
        id: dialingPopup
        anchors.fill: parent
        visible: showDialingFromSearch
        z: 98
        contactName:     callingName
        contactInitials: callingInitials
        contactPhone:    callingPhone
        contactColor:    callingColor
        onDismissed:   showDialingFromSearch = false
        onCallClicked: {
            showDialingFromSearch = false
            incomingCall.callLabel = "Calling..."
            incomingCall.show(callingName, callingInitials, callingPhone, callingColor)
        }
    }

}
