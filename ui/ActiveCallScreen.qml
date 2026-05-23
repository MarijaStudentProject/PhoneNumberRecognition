import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root
    color: "#0a4a4a"

    property string contactName: ""
    property string contactInitials: ""
    property string contactPhone: ""
    property string contactColor: "#B4C8E8"
    property string callLabel: "Connecting..."

    // states: "calling", "active"
    property string callState: "calling"
    property int elapsedSeconds: 0

    signal callEnded()

    visible: false

    function show(name, initials, phone, color) {
        contactName     = name
        contactInitials = initials
        contactPhone    = phone
        contactColor    = color
        callState       = "calling"
        elapsedSeconds  = 0
        visible         = true
    }

    function formatTime(s) {
        var m = Math.floor(s / 60)
        var sec = s % 60
        return (m < 10 ? "0" + m : m) + ":" + (sec < 10 ? "0" + sec : sec)
    }

    // Simulate connecting after 2 seconds
    Timer {
        id: dialingTimer
        interval: 2000
        running:{ root.visible && callState === "calling"}
        onTriggered: {callState = "active"; dialingSound.stop()}

    }

    // Call timer
    Timer {
        interval: 1000
        repeat: true
        running: callState === "active"
        onTriggered: {elapsedSeconds++; }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 48
        anchors.bottomMargin: 32
        spacing: 0

        // Status label
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: callState === "calling" ? "CONNECTING.." : "ON CALL"
            font.pixelSize: 13
            font.weight: Font.Medium
            font.letterSpacing: 2
            color: "#80ffffff"
        }

        // Name
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 8
            text: root.contactName.length > 0 ? root.contactName : root.contactPhone
            font.pixelSize: root.contactName.length > 0 ? 28 : 22
            font.weight: Font.Bold
            color: "white"
        }

        // Phone
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 4
            text: root.contactPhone
            font.pixelSize: 14
            color: "#80ffffff"
            visible: root.contactName.length > 0
        }

        // Avatar
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 32
            width: 120; height: 120; radius: 60
            color: root.contactColor

            Text {
                anchors.centerIn: parent
                text: root.contactInitials
                font.pixelSize: 40
                font.weight: Font.Medium
                color: "#1a1a1a"
            }
        }

        Item { Layout.fillHeight: true }

        // Timer
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 16
            text: callState === "calling" ? "" : formatTime(elapsedSeconds)
            font.pixelSize: 14
            color: "#80ffffff"
        }

        // Action buttons grid
        GridLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 32
            columns: 3
            rowSpacing: 24
            columnSpacing: 24

            Repeater {
                model: [
                    { icon: "\uf131", label: "Mute" },
                    { icon: "\uf11c", label: "Keypad" },
                    { icon: "\uf028", label: "Speaker" },
                    { icon: "\uf067", label: "Add" },
                    { icon: "\uf03d", label: "Video" },
                    { icon: "\uf04c", label: "Hold" },
                ]

                delegate: ColumnLayout {
                    spacing: 6

                    property bool active: false  // ← toggle state

                    Rectangle {
                        width: 60; height: 60; radius: 30
                        color: active ? "#0d3333" : "#1a5a5a"  // ← darker when active
                        Layout.alignment: Qt.AlignHCenter

                        Text {
                            anchors.centerIn: parent
                            text: modelData.icon
                            font.family: fontAwesome.name
                            font.pixelSize: 20
                            color: active ? "#4db8b8" : "white"  // ← tinted when active
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                parent.parent.active = !parent.parent.active
                                clickSound.play()
                            }
                        }
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: modelData.label
                        font.pixelSize: 12
                        color: "#ccffffff"
                    }
                }
            }
        }

        // End call button
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 64; height: 64; radius: 32
            color: "#E53935"

            Text {
                anchors.centerIn: parent
                font.family: fontAwesome.name
                text: "\uf3dd"
                font.pixelSize: 26
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.visible = false
                    clickSound.play()
                    root.callEnded()
                }
            }
        }
    }
}
