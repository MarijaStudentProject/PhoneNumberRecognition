import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#00000066"

    property string contactName: ""
    property string contactInitials: ""
    property string contactPhone: ""
    property string contactColor: "#E8B4B8"

    signal dismissed()
    signal callClicked(string phone)
    MouseArea {
        anchors.fill: parent
        onClicked: root.dismissed()
    }

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width
        height: 280
        radius: 20
        color: "white"

        MouseArea { anchors.fill: parent }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 0

            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 36; height: 4; radius: 2
                color: "#E0E0E0"
                Layout.bottomMargin: 20
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 16
                Layout.bottomMargin: 24

                Rectangle {
                    width: 52; height: 52; radius: 26
                    color: root.contactColor

                    Text {
                        anchors.centerIn: parent
                        text: root.contactInitials
                        font.pixelSize: 18
                        font.weight: Font.Medium
                        color: "white"
                    }
                }

                ColumnLayout {
                    spacing: 2
                    Text {
                        text: root.contactName
                        font.pixelSize: 17
                        font.weight: Font.Medium
                        color: "#1a1a1a"
                    }
                    Text {
                        text: root.contactPhone
                        font.pixelSize: 14
                        color: "#888"
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 16

                Rectangle {
                    Layout.fillWidth: true
                    height: 52
                    radius: 26
                    color: "#F2F2F2"

                    Text {
                        anchors.centerIn: parent
                        text: "Cancel"
                        font.pixelSize: 16
                        color: "#1a1a1a"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {root.dismissed(); clickSound.play()}
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 52
                    radius: 26
                    color: "#4CAF50"

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 8
                        Text { text: "📞"; font.pixelSize: 16 }
                        Text {
                            text: "Call"
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            color: "white"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                            root.callClicked(root.contactPhone)
                            root.dismissed()
                            clickSound.play()
                        }
                    }
                }
            }
        }
    }
}
