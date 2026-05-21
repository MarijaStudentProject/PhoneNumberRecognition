import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: "#F8F8F8"

    property string dialedNumber: ""
    property var contactsModel: null

    signal contactCallRequested(string name, string initials, string phone, string color)

    onDialedNumberChanged: contactsModel.setSearchText(dialedNumber)

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

    // Matched contacts list — fills space above search bar
    ListView {
        id: matchList
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: searchBar.top
        clip: true
        model: contactsModel
        visible: matchList.count > 0 && dialedNumber.length > 0

        delegate: Rectangle {
            width: matchList.width
            height: 64
            color: "white"

            readonly property string displayName:     fullName(model.name, model.surname)
            readonly property string displayInitials: initialsFrom(model.name, model.surname)
            readonly property string displayColor:    colorForId(model.contactId)

            Row {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                spacing: 14

                Rectangle {
                    width: 42; height: 42; radius: 21
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

                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 2
                    Text { text: displayName;  font.pixelSize: 15; color: "#1a1a1a" }
                    Text { text: model.phone;  font.pixelSize: 13; color: "#888" }
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
                    contactCallRequested(displayName, displayInitials, model.phone, displayColor)
                    clickSound.play()
                }
            }
        }
    }

    // No matches placeholder
    Item {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: searchBar.top
        visible: matchList.count === 0

        Text {
            anchors.centerIn: parent
            text: dialedNumber.length === 0 ? "" : "No matches"
            font.pixelSize: 14
            color: "#AAAAAA"
        }
    }

    // Search bar — fixed above keypad
    Rectangle {
        id: searchBar
        anchors.bottom: keypad.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.bottomMargin: 8
        height: 48
        radius: 24
        color: "white"
        border.color: "#E0E0E0"
        border.width: 1

        Row {
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 12

            Text {
                width: parent.width - 48
                height: parent.height
                text: dialedNumber.length > 0 ? dialedNumber : ""
                font.pixelSize: 22
                font.letterSpacing: 2
                color: "#1a1a1a"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                Text {
                    anchors.fill: parent
                    text: "Enter number..."
                    font.pixelSize: 15
                    color: "#AAAAAA"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    visible: dialedNumber.length === 0
                }
            }

            Rectangle {
                width: 36; height: 36
                anchors.verticalCenter: parent.verticalCenter
                radius: 18
                color: "#F2F2F2"
                visible: dialedNumber.length > 0

                Text {
                    anchors.centerIn: parent
                    text: "\uf55a"
                    font.pixelSize: 18
                    color: "#555"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: { dialedNumber = dialedNumber.slice(0, -1); clickSound.play() }
                    onPressAndHold: dialedNumber = ""
                }
            }
        }
    }

    // Keypad — fixed above call button
    Item {
        id: keypad
        anchors.bottom: callButton.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: 8
        height: 300

        GridLayout {
            anchors.centerIn: parent
            columns: 3
            rowSpacing: 4
            columnSpacing: 0

            Repeater {
                model: [
                    { digit: "1", sub: "   " },
                    { digit: "2", sub: "ABC" },
                    { digit: "3", sub: "DEF" },
                    { digit: "4", sub: "GHI" },
                    { digit: "5", sub: "JKL" },
                    { digit: "6", sub: "MNO" },
                    { digit: "7", sub: "PQRS" },
                    { digit: "8", sub: "TUV" },
                    { digit: "9", sub: "WXYZ" },
                    { digit: "*", sub: "" },
                    { digit: "0", sub: "+" },
                    { digit: "#", sub: "" },
                ]

                delegate: Rectangle {
                    width: 100
                    height: 68
                    color: "transparent"

                    Column {
                        anchors.centerIn: parent
                        spacing: 1
                        width: 80
                        Text {
                            horizontalAlignment: Text.AlignHCenter
                            width: parent.width
                            text: modelData.digit
                            font.pixelSize: 28
                            color: "#1a1a1a"
                        }
                        Text {
                            horizontalAlignment: Text.AlignHCenter
                            width: parent.width
                            text: modelData.sub
                            font.pixelSize: 10
                            color: "#888"
                            visible: modelData.sub.length > 0
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            dialedNumber += modelData.digit
                            switch(modelData.digit) {
                                case "0": dialPad0.play(); break;
                                case "1": dialPad1.play(); break;
                                case "2": dialPad2.play(); break;
                                case "3": dialPad3.play(); break;
                                case "4": dialPad4.play(); break;
                                case "5": dialPad5.play(); break;
                                case "6": dialPad6.play(); break;
                                case "7": dialPad7.play(); break;
                                case "8": dialPad8.play(); break;
                                case "9": dialPad9.play(); break;
                                default:  dialPad0.play();
                            }
                        }
                        onPressAndHold: {
                            if (modelData.digit === "0") dialedNumber += "+"
                        }
                    }
                }
            }
        }
    }

    // Call button — fixed at bottom
    Rectangle {
        id: callButton
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 24
        width: 64; height: 64; radius: 32
        color: "#4CAF50"

        Text {
            anchors.centerIn: parent
            text: "\uf095"
            font.pixelSize: 24
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                detailesContactModel.clear()
                contactsModel.makeCall()
                clickSound.play()
                detailesContactModel.name
                    ? activeCall.show(
                        detailesContactModel.name + " " + detailesContactModel.surname,
                        initialsFrom(detailesContactModel.name, detailesContactModel.surname),
                        detailesContactModel.phoneNumbers[0], "#C4B4E8")
                    : activeCall.show("", "", dialedNumber, "#C4B4E8")
            }
        }
    }
}
