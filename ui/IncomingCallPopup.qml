import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {

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
    
    id: root
    color: "#80000000"

    property string contactName: ""
    property string contactInitials: ""
    property string contactPhone: ""
    property string contactColor: "#E8B4B8"
    property string callLabel: "Incoming Call"
    property int contactCountryCode: 0
    signal accepted()
    signal declined()

    function countryCodeToName(code) {
        var map = {
            1: "USA / Canada", 7: "Russia", 20: "Egypt",
            27: "South Africa", 30: "Greece", 31: "Netherlands",
            32: "Belgium", 33: "France", 34: "Spain", 36: "Hungary",
            39: "Italy", 40: "Romania", 41: "Switzerland", 43: "Austria",
            44: "United Kingdom", 45: "Denmark", 46: "Sweden", 47: "Norway",
            48: "Poland", 49: "Germany", 51: "Peru", 52: "Mexico",
            54: "Argentina", 55: "Brazil", 56: "Chile", 57: "Colombia",
            61: "Australia", 62: "Indonesia", 63: "Philippines",
            64: "New Zealand", 65: "Singapore", 66: "Thailand",
            81: "Japan", 82: "South Korea", 86: "China", 90: "Turkey",
            91: "India", 92: "Pakistan", 94: "Sri Lanka", 98: "Iran",
            212: "Morocco", 213: "Algeria", 216: "Tunisia",
            234: "Nigeria", 254: "Kenya",
            380: "Ukraine", 381: "Serbia", 382: "Montenegro", 385: "Croatia",
            386: "Slovenia", 387: "Bosnia & Herzegovina",
            389: "North Macedonia", 420: "Czech Republic", 421: "Slovakia",
            994: "Azerbaijan", 995: "Georgia", 998: "Uzbekistan"
        }
        return map[code] !== undefined ? map[code] : ""
    }

    function show(name, initials, phone, color, countryCode) {
        contactName        = name
        contactInitials    = initials
        contactPhone       = phone
        contactColor       = color
        contactCountryCode = countryCode !== undefined ? countryCode : 0
        visible = true
    }
    function showSelected() {
        contactName        = detailesContactModel.name + " " + detailesContactModel.surname
        contactInitials    = initialsFrom(detailesContactModel.name, detailesContactModel.surname)
        contactPhone       = detailesContactModel.getPrimaryNumber()
        contactColor       =  colorForId(detailesContactModel.contactId) 
        contactCountryCode = detailesContactModel.getPrimaryCountryCode() !== undefined ? detailesContactModel.getPrimaryCountryCode() : 0
        visible = true
    }

    visible: false

    // Slide-in animation
    Rectangle {
        id: card
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 24
        width: parent.width - 32
        height: cardContent.implicitHeight + 48
        radius: 20
        color: "white"

        // Drop shadow
        layer.enabled: true
        layer.effect: null

        // Entrance animation
        NumberAnimation on anchors.topMargin {
            from: -200
            to: 24
            duration: 350
            easing.type: Easing.OutCubic
            running: root.visible
        }

        ColumnLayout {
            id: cardContent
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 24
            spacing: 0

            // "Incoming call" label
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                Layout.bottomMargin: 8
                text: root.callLabel        // ← was hardcoded "Incoming Call"
                font.pixelSize: 13
                color: "#888"
                font.weight: Font.Medium
            }

            // Avatar
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 12
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
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: root.contactName
                font.pixelSize: 18
                font.weight: Font.Medium
                color: "#1a1a1a"
            }

            // Country
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 2
                text: countryCodeToName(root.contactCountryCode)
                font.pixelSize: 12
                color: "#aaa"
                visible: root.contactCountryCode !== 0
            }

            // Phone
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 4
                Layout.bottomMargin: 24
                text: root.contactPhone
                font.pixelSize: 14
                color: "#888"
            }

            // Divider
            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#F0F0F0"
                Layout.bottomMargin: 20
            }

            // Accept + Decline buttons
            RowLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 24
                spacing: 16

                // Decline
                Rectangle {
                    Layout.fillWidth: true
                    height: 52
                    radius: 26
                    color: "#FFEBEE"

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 2
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: "\uf3dd"
                            font.pixelSize: 18
                            font.family: fontAwesome.name;
                        }
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: "Decline"
                            font.pixelSize: 12
                            color: "#F44336"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.visible = false
                            root.declined()
                            clickSound.play()
                        }
                    }
                }

                // Accept
                Rectangle {
                    Layout.fillWidth: true
                    height: 52
                    radius: 26
                    color: "#E8F5E9"

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 2
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: "\uf095"
                            font.pixelSize: 18
                            font.family: fontAwesome.name;
                        }
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: "Accept"
                            font.pixelSize: 12
                            color: "#4CAF50"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            root.visible = false
                            root.accepted()
                            clickSound.play()
                        }
                    }
                }
            }
        }
    }
}
