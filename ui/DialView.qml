import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: "#F8F8F8"

    property string dialedNumber: ""
    property var contactsModel: null

    // use our function for phone number search instead of this
    function findMatches(number) {
        var results = []
        if (!contactsModel || number.length < 2) return results
        var digits = number.replace(/\D/g, "")
        for (var i = 0; i < contactsModel.count; i++) {
            var c = contactsModel.get(i)
            var phoneDigits = c.phone.replace(/\D/g, "")
            if (phoneDigits.includes(digits)) {
                results.push(c)
            }
        }
        return results
    }

    property var matches: []

    onDialedNumberChanged: matches = findMatches(dialedNumber)

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Matched contacts list
        ListView {
            id: matchList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: matches
            visible: matches.length > 0

            delegate: Rectangle {
                width: matchList.width
                height: 64
                color: "white"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    spacing: 14

                    Rectangle {
                        width: 42; height: 42; radius: 21
                        color: modelData.color
                        Text {
                            anchors.centerIn: parent
                            text: modelData.initials
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            color: "white"
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text { text: modelData.name;  font.pixelSize: 15; color: "#1a1a1a" }
                        Text { text: modelData.phone; font.pixelSize: 13; color: "#888" }
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
            }
        }

        // Placeholder when no matches and nothing typed
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: matches.length === 0

            Text {
                anchors.centerIn: parent
                text: dialedNumber.length === 0 ? "" : "No matches"
                font.pixelSize: 14
                color: "#AAAAAA"
            }
        }

        // Dialed number display
        Item {
            Layout.fillWidth: true
            height: 60

            Text {
                anchors.centerIn: parent
                text: dialedNumber
                font.pixelSize: 28
                font.letterSpacing: 4
                color: "#1a1a1a"
            }
        }

        // Keypad
        GridLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 8
            columns: 3
            rowSpacing: 4
            columnSpacing: 0

            Repeater {
                model: [
                    { digit: "1", sub: "" },
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
                    height: 72
                    color: "transparent"

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 1

                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: modelData.digit
                            font.pixelSize: 28
                            color: "#1a1a1a"
                        }
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: modelData.sub
                            font.pixelSize: 10
                            color: "#888"
                            visible: modelData.sub.length > 0
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: dialedNumber += modelData.digit
                        onPressAndHold: {
                            if (modelData.digit === "0") dialedNumber += "+"
                        }
                    }
                }
            }
        }

        // Call + backspace row
        RowLayout {
            Layout.fillWidth: true
            Layout.bottomMargin: 24
            Layout.leftMargin: 24
            Layout.rightMargin: 24

            Item { Layout.fillWidth: true }

            Rectangle {
                width: 64; height: 64; radius: 32
                color: "#4CAF50"

                Text {
                    anchors.centerIn: parent
                    text: "📞"
                    font.pixelSize: 24
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: { /* handle call */ }
                }
            }

            Item { Layout.fillWidth: true }

            Text {
                text: "⌫"
                font.pixelSize: 24
                color: dialedNumber.length > 0 ? "#1a1a1a" : "#CCCCCC"

                MouseArea {
                    anchors.fill: parent
                    onClicked: dialedNumber = dialedNumber.slice(0, -1)
                    onPressAndHold: dialedNumber = ""
                }
            }

            Item { Layout.fillWidth: true }
        }
    }
}
