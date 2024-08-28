import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Dialogs

Window {
    width: 1920 / 2
    height: 1080 / 2
    title: qsTr("Play Video")
    visible: false

    property MediaPlayer mediaPlayer:mediaPlayer
    property string videoId

    property LoginWindow gLoginWindow

    onClosing: {
        mediaPlayer.stop()
    }

    // 显示错误信息
    Popup {
        id: mediaError
        anchors.centerIn: Overlay.overlay
        Text {
            id: mediaErrorText
        }
    }

    MediaPlayer {
        id: mediaPlayer
        videoOutput: videoOutput
        audioOutput: AudioOutput {
            id: audio
            muted: playbackControl.muted
            volume: playbackControl.volume
        }

        onErrorOccurred: { mediaErrorText.text = mediaPlayer.errorString; mediaError.open() } // 发生错误

    }

    VideoOutput {
        id: videoOutput
        property bool fullScreen: false

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        TapHandler {
            onDoubleTapped: {
                parent.fullScreen ? showNormal() : showFullScreen()
                parent.fullScreen = !parent.fullScreen
            }
        }
    }

    PlaybackControl {
        id: playbackControl

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        mediaPlayer: mediaPlayer
        video_id: videoId

        loginWindow: gLoginWindow
    }

    Timer {
        id: playbackControlTimer
        interval: 3000
        onTriggered: playbackControl.visible = false
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: {
            if(mouseY > (height - playbackControl.height))
                playbackControl.visible = true
            else {
                playbackControlTimer.stop()
                playbackControlTimer.start()
            }
        }
        acceptedButtons: Qt.NoButton
    }

}
