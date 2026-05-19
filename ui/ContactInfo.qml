// import QtQuick 2.15
// import QtQuick.Controls 2.15
// import QtQuick.Layouts 1.15

// Rectangle {
//     id: root
//     color: "#80000000"

//     property string contactName: ""
//     property string contactInitials: ""
//     property string contactPhone: ""
//     property string contactColor: "#E8B4B8"
//     property string contactEmail: ""
//     property string contactAddress: ""
//     property string contactBirthday: ""

//     signal dismissed()
//     signal callClicked(string phone)

//     // Dim background tap to dismiss
//     MouseArea {
//         anchors.fill: parent
//         onClicked: root.dismissed()
//     }

//     // Floating card
//     Rectangle {
//         anchors.centerIn: parent
//         width: parent.width - 48
//         height: cardContent.implicitHeight + 48
//         radius: 16
//         color: "white"

//         // Block background taps from passing through
//         MouseArea { anchors.fill: parent }

//         // Close button
//         Text {
//             anchors.top: parent.top
//             anchors.right: parent.right
//             anchors.topMargin: 12
//             anchors.rightMargin: 14
//             text: "✕"
//             font.pixelSize: 14
//             color: "#888"
//             MouseArea {
//                 anchors.fill: parent
//                 onClicked: root.dismissed()
//             }
//         }

//         ColumnLayout {
//             id: cardContent
//             anchors.top: parent.top
//             anchors.left: parent.left
//             anchors.right: parent.right
//             anchors.margins: 24
//             spacing: 0

//             // Avatar
//             Rectangle {
//                 Layout.alignment: Qt.AlignHCenter
//                 Layout.topMargin: 24
//                 width: 72; height: 72; radius: 36
//                 color: root.contactColor

//                 Text {
//                     anchors.centerIn: parent
//                     text: root.contactInitials
//                     font.pixelSize: 24
//                     font.weight: Font.Medium
//                     color: "white"
//                 }
//             }

//             // Name
//             Text {
//                 Layout.alignment: Qt.AlignHCenter
//                 Layout.topMargin: 12
//                 Layout.bottomMargin: 16
//                 text: root.contactName
//                 font.pixelSize: 18
//                 font.weight: Font.Medium
//                 color: "#1a1a1a"
//             }

//             // Call + favorite buttons
//             RowLayout {
//                 Layout.alignment: Qt.AlignHCenter
//                 Layout.bottomMargin: 20
//                 spacing: 12

//                 Rectangle {
//                     width: 90; height: 34; radius: 17
//                     color: "#00897B"

//                     RowLayout {
//                         anchors.centerIn: parent
//                         spacing: 6
//                         Text { text: "📞"; font.pixelSize: 13 }
//                         Text {
//                             text: "Call"
//                             font.pixelSize: 14
//                             font.weight: Font.Medium
//                             color: "white"
//                         }
//                     }

//                     MouseArea {
//                         anchors.fill: parent
//                         onClicked: root.callClicked(root.contactPhone)
//                     }
//                 }

//                 Rectangle {
//                     width: 34; height: 34; radius: 17
//                     color: "#F2F2F2"
//                     Text {
//                         anchors.centerIn: parent
//                         text: "☆"
//                         font.pixelSize: 16
//                         color: "#888"
//                     }
//                 }
//             }

//             // Divider
//             Rectangle {
//                 Layout.fillWidth: true
//                 height: 1
//                 color: "#F0F0F0"
//                 Layout.bottomMargin: 16
//             }

//             // Phone
//             ColumnLayout {
//                 Layout.fillWidth: true
//                 Layout.bottomMargin: 14
//                 spacing: 2
//                 Text { text: "PHONE"; font.pixelSize: 11; color: "#888" }
//                 Text { text: root.contactPhone; font.pixelSize: 14; color: "#1a1a1a" }
//             }

//             // Email
//             ColumnLayout {
//                 Layout.fillWidth: true
//                 Layout.bottomMargin: 14
//                 spacing: 2
//                 visible: root.contactEmail.length > 0
//                 Text { text: "EMAIL"; font.pixelSize: 11; color: "#888" }
//                 Text { text: root.contactEmail; font.pixelSize: 14; color: "#1a1a1a" }
//             }

//             // Address
//             ColumnLayout {
//                 Layout.fillWidth: true
//                 Layout.bottomMargin: 14
//                 spacing: 2
//                 visible: root.contactAddress.length > 0
//                 Text { text: "ADDRESS"; font.pixelSize: 11; color: "#888" }
//                 Text { text: root.contactAddress; font.pixelSize: 14; color: "#1a1a1a" }
//             }

//             // Birthday
//             ColumnLayout {
//                 Layout.fillWidth: true
//                 Layout.bottomMargin: 24
//                 spacing: 2
//                 visible: root.contactBirthday.length > 0
//                 Text { text: "BIRTHDAY"; font.pixelSize: 11; color: "#888" }
//                 Text { text: root.contactBirthday; font.pixelSize: 14; color: "#1a1a1a" }
//             }
//         }
//     }
// }
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#80000000"

    property string contactName: ""
    property string contactInitials: ""
    property string contactPhone: ""
    property string contactColor: "#E8B4B8"
    property string contactEmail: ""
    property string contactAddress: ""
    property string contactBirthday: ""

    signal dismissed()
    signal callClicked(string phone)

    MouseArea {
        anchors.fill: parent
        onClicked: root.dismissed()
    }

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - 48
        height: cardContent.implicitHeight + 48
        radius: 16
        color: "white"

        MouseArea { anchors.fill: parent }

        Text {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 12
            anchors.rightMargin: 14
            text: "✕"
            font.pixelSize: 14
            color: "#888"
            MouseArea {
                anchors.fill: parent
                onClicked: root.dismissed()
            }
        }

        ColumnLayout {
            id: cardContent
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 24
            spacing: 0

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

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 12
                Layout.bottomMargin: 16
                text: root.contactName
                font.pixelSize: 18
                font.weight: Font.Medium
                color: "#1a1a1a"
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 20
                spacing: 12

                Rectangle {
                    width: 90; height: 34; radius: 17
                    color: "#00897B"

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 6
                        Text { text: "📞"; font.pixelSize: 13 }
                        Text {
                            text: "Call"
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            color: "white"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.callClicked(root.contactPhone)
                    }
                }

                Rectangle {
                    width: 34; height: 34; radius: 17
                    color: "#F2F2F2"
                    Text {
                        anchors.centerIn: parent
                        text: "☆"
                        font.pixelSize: 16
                        color: "#888"
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#F0F0F0"
                Layout.bottomMargin: 16
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 14
                spacing: 2
                Text { text: "PHONE"; font.pixelSize: 11; color: "#888" }
                Text { text: root.contactPhone; font.pixelSize: 14; color: "#1a1a1a" }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 14
                spacing: 2
                visible: root.contactEmail.length > 0
                Text { text: "EMAIL"; font.pixelSize: 11; color: "#888" }
                Text { text: root.contactEmail; font.pixelSize: 14; color: "#1a1a1a" }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 14
                spacing: 2
                visible: root.contactAddress.length > 0
                Text { text: "ADDRESS"; font.pixelSize: 11; color: "#888" }
                Text { text: root.contactAddress; font.pixelSize: 14; color: "#1a1a1a" }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 24
                spacing: 2
                visible: root.contactBirthday.length > 0
                Text { text: "BIRTHDAY"; font.pixelSize: 11; color: "#888" }
                Text { text: root.contactBirthday; font.pixelSize: 14; color: "#1a1a1a" }
            }
        }
    }
}
