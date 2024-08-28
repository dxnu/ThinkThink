import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia


Item {
    id: root

    property MediaPlayer mediaPlayer
    property int mediaPlayerState: mediaPlayer.playbackState
    property alias muted: audio.muted
    property alias volume: audio.volume

    property string video_id

    property LoginWindow loginWindow

    height: frame.height

    opacity: 0.7

    CommentWindow {
        id: commentWindow

        gLoginWindow: loginWindow
    }


    Frame {
        id: frame
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        background: Rectangle {
            color: "#50616d"
        }

        Connections {
            target: mediaPlayer
            function onPlaybackStateChanged(placeBackState) {
                if(placeBackState === MediaPlayer.StoppedState) {
                    controlButtonBackground.text = qsTr("\u25B8")
                    controlButton.playing = false
                }
            }
        }

        ColumnLayout {
            id: playbackControlPanel
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            PlaybackSeekControl {
                Layout.fillWidth: true
                mediaPlayer: root.mediaPlayer
            }

            RowLayout {
                id: playerButtons

                RoundButton {
                    id: controlButton
                    radius: 50.0
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 50

                    property bool playing: true
                    background: Text {
                        id: controlButtonBackground
                        text: qsTr("\u2016")
                        font.pixelSize: Qt.application.font.pixelSize * (text === qsTr("\u25B8") ? 3.2 : 1.8)
                        color: controlButton.hovered ? "#0488d0" : "#000000"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        if(playing) {
                            controlButtonBackground.text = qsTr("\u25B8")
                            mediaPlayer.pause()
                        } else {
                            controlButtonBackground.text = qsTr("\u2016")
                            mediaPlayer.play()
                        }
                        playing = !playing
                    }
                }

                Text {
                    id: mediaTime
                    Layout.minimumWidth: 50
                    Layout.minimumHeight: 18
                    horizontalAlignment: Text.AlignRight
                    text: {
                        var m = Math.floor(mediaPlayer.position / 60000)
                        var ms = (mediaPlayer.position / 1000 - m * 60).toFixed(1)
                        var dm = Math.floor(mediaPlayer.duration / 60000)
                        var dms = (mediaPlayer.duration / 1000 - m * 60).toFixed(1)
                        return `${m}:${ms.padStart(4, 0)}/${dm}:${dms}`
                    }
                }

                Button {
                    text: qsTr("评论")

                    onClicked: {
                        // 获取评论
                        videoCommentController.getComments(video_id)

                        commentWindow.videoId = video_id
                        commentWindow.modality = Qt.WindowModal
                        commentWindow.show()
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                PlackbackRateControl {
                    Layout.minimumWidth: 100
                    Layout.maximumWidth: 150
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    mediaPlayer: root.mediaPlayer
                }

                Item {
                    Layout.fillWidth: true
                }

                // Audio Control
                AudioControl {
                    id: audio
                    Layout.minimumWidth: 100
                    Layout.maximumWidth: 150
                    Layout.fillWidth: true
                    mediaPlayer: root.mediaPlayer
                }
            }
        }


    }
}
