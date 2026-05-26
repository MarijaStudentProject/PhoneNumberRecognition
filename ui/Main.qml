import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia




ApplicationWindow {


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

    SoundEffect {
        id: clickSound
        source: "qrc:/PhoneApp/ui/sounds/click.wav"
    }

    // Dialing tone — loops while calling
    SoundEffect {
        id: keyboardSound
        source: "qrc:/PhoneApp/ui/sounds/keyboard-sound.wav"
    }
    SoundEffect {
        id: ringtoneSound
        source: "qrc:/PhoneApp/ui/sounds/samsung-ringtone.wav"
        loops: SoundEffect.Infinite
    }
    SoundEffect {
        id: dialingSound
        source: "qrc:/PhoneApp/ui/sounds/dialing-sound.wav"
        loops: SoundEffect.Infinite
    }
    SoundEffect {
        id: dialPad0
        source: "qrc:/PhoneApp/ui/sounds/dialpad-0.wav"
    }
    SoundEffect {
        id: dialPad1
        source: "qrc:/PhoneApp/ui/sounds/dialpad-1.wav"
    }
    SoundEffect {
        id: dialPad2
        source: "qrc:/PhoneApp/ui/sounds/dialpad-2.wav"
    }
    SoundEffect {
        id: dialPad3
        source: "qrc:/PhoneApp/ui/sounds/dialpad-3.wav"
    }
    SoundEffect {
        id: dialPad4
        source: "qrc:/PhoneApp/ui/sounds/dialpad-4.wav"
    }
    SoundEffect {
        id: dialPad5
        source: "qrc:/PhoneApp/ui/sounds/dialpad-5.wav"
    }
    SoundEffect {
        id: dialPad6
        source: "qrc:/PhoneApp/ui/sounds/dialpad-6.wav"
    }
    SoundEffect {
        id: dialPad7
        source: "qrc:/PhoneApp/ui/sounds/dialpad-7.wav"
    }
    SoundEffect {
        id: dialPad8
        source: "qrc:/PhoneApp/ui/sounds/dialpad-8.wav"
    }
    SoundEffect {
        id: dialPad9
        source: "qrc:/PhoneApp/ui/sounds/dialpad-9.wav"
    }

    function resolveNumber(number, country) {
        detailesContactModel.clear()
        cppContactsModel.call(number, country)
        var norm = cppContactsModel.normalizeIncoming(number, country)
        callingPhoneNormalized = norm.normalized
        callingPhoneCode = norm.countryCode
    }

    function makeOutgoingCall(number, country) {
        if (number.length === 0) return
        resolveNumber(number, country)
        detailesContactModel.contactId !== -1
            ? activeCall.showSelected()
            : activeCall.show("", "", callingPhoneNormalized ? callingPhoneNormalized : number, "#C4B4E8", callingPhoneCode)
    }

    function receiveIncomingCall(number, country) {
        if (number.length === 0) return
        resolveNumber(number, country)
        detailesContactModel.contactId !== -1
            ? incomingCall.showSelected()
            : incomingCall.show("", "", callingPhoneNormalized ? callingPhoneNormalized : number, "#C4B4E8", callingPhoneCode)
    }

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
    property string callingPhoneNormalized: ""
    property int callingPhoneCode: 0
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
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            currentTab = 0
                            dialView.dialedNumber = ""
                            clickSound.play()
                        }
                    }
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
                    MouseArea { anchors.fill: parent;
                            onClicked: {
                                currentTab = 1
                                clickSound.play()
                            }
                    }
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
                    incomingCall.callLabel = "Incoming call"
                    activeCall.showSelected()
                }
            }
            DialView {
                    id: dialView
                    contactsModel: cppContactsModel
                    onContactCallRequested: (name, initials, phone, color) => {
                        activeCall.showSelected()
                        showDialingFromSearch = false
                    }
                    onCallButtonPressed: (phone) => makeOutgoingCall(phone, "RS")
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
            onClicked: {incomingCall.show("Beatriz Souza", "BS", "(917) 555-0144", "#C4B4E8", 0); clickSound.play()}
        }
    }

    // Incoming call popup — overlays everything
    IncomingCallPopup {

        id: incomingCall
        anchors.fill: parent
        z: 99
        onAccepted: {
            activeCall.show(incomingCall.contactName, incomingCall.contactInitials,
                            incomingCall.contactPhone, incomingCall.contactColor, incomingCall.contactCountryCode);
        }
        onDeclined: callPoller.resumePolling()
        onVisibleChanged: {
                if (visible)
                    ringtoneSound.play()
                else
                    ringtoneSound.stop()
            }
    }
    ActiveCallScreen {
        id: activeCall
        anchors.fill: parent
        z: 98
        onVisibleChanged: {
            if(visible) {
                dialingSound.play();
            } else {
                dialingSound.stop();
            }
        }

        onCallEnded: callPoller.resumePolling()
    }

    Connections {
        target: callPoller
        function onCallReceived(number, country) {
            clickSound.play()
            detailesContactModel.clear()
            cppContactsModel.call(number,country)
            detailesContactModel.name? incomingCall.show(
                        detailesContactModel.name + " " + detailesContactModel.surname,
                        initialsFrom(detailesContactModel.name, detailesContactModel.surname),
                        detailesContactModel.phoneNumbers[0], "#C4B4E8")
                    : incomingCall.show("", "", number, "#C4B4E8")
            
        }
    }

    FontLoader {
        id: fontAwesome
        source: "qrc:PhoneApp/ui/fonts/fa-solid-900.ttf"
        onStatusChanged: {
               if (status === FontLoader.Ready)
                   console.log("Font loaded:", name)
               else if (status === FontLoader.Error)
                   console.log("Font failed to load")
           }
    }

}
