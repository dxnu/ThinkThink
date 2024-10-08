﻿import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Item {
	id: root

	property MediaPlayer mediaPlayer
	property bool muted: false
    property real volume: volumeSlider.value / 100.

    implicitHeight: buttons.height



    RowLayout {
        anchors.fill: parent

        Item {
            id: buttons

            width: muteButton.implicitWidth
            height: muteButton.implicitHeight

            RoundButton {
                id: muteButton
                radius: 50.0
                icon.source: muted ? "../images/Mute_Icon.svg" : "../images/Speaker_Icon.svg"
                onClicked: { muted = !muted }
            }
        }

        Slider {
            id: volumeSlider
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            enabled: true
            to: 100.0
            value: 100.0
        }
    }
}
