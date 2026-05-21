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
    property string callLabel: "Incoming Call"
    signal accepted()
    signal declined()


    function show(name, initials, phone, color) {
        contactName     = name
        contactInitials = initials
        contactPhone    = phone
        contactColor    = color
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
