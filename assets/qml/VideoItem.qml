import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    width: parent.width
    height: videoItemColumn.height

    property bool editable: false
    property string showText

    property LoginWindow loginWindow

//    VideoItemFeeds {
//        id: videoItemFeeds
//    }

//    property string popup_text_1

//    Popup {
//        id: popup
//        x: 100
//        y: 100
//        width: 200
//        height: 200
//        modal: true
//        focus: true
//        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

//        contentItem: Text {
//            text: popup_text_1
//        }
//    }
    VideoPlayer {
        id: videoPlayer

        gLoginWindow: loginWindow
    }


    // 每个VideoItem列表
    Column {
        id: videoItemColumn
        width: parent.width
        spacing: 15


        // 分区标题区域
        RowLayout {
            id: videoItemHeader
            width: parent.width
            height: 20


            Text {
                text: showText
                color: "#ffffff"
                font.pointSize: 12
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
               text: qsTr("换一换")

               contentItem: Text {
                   text: parent.text
                   font: parent.font
                   color: "#ffffff"
                   horizontalAlignment: Text.AlignHCenter
                   verticalAlignment: Text.AlignVCenter
                   elide: Text.ElideRight
               }

               background: Rectangle {
                   color: "green"
                   border.color: "red"
                   border.width: 1
                   radius: 2
               }

               visible: false
           }

            Button {
                text: qsTr("更多")
                Layout.preferredWidth: 60
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 10

                visible: false
            }
        }

        //////////////////////////////
        GridView {
            id: videoGridView
            width: parent.width
            height: cellHeight * (Math.ceil(videoGridView.count / Math.floor(parent.width / cellWidth)))
            cellWidth: 270
            cellHeight: 165
            clip: true

            model: videoController.model()/*videoItemFeeds*/
            delegate: videoViewDelegate
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        }

        Component {
            id: videoViewDelegate

            Item {
                width: videoGridView.cellWidth
                height: videoGridView.cellHeight

                Column {
                    anchors.fill: parent

                    Image {
                        id: videoImage
                        width: parent.width - 5
                        height: parent.height - 60
                        asynchronous: true
                        //fillMode: Image.PreserveAspectFit
                        cache: true
                        //source: "../../images/1209425.png"
                        source: model.cover_image

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                videoGridView.currentIndex = index
                                videoPlayer.title = model.video_name
                                videoPlayer.visible = true
                                videoPlayer.mediaPlayer.stop()
                                videoPlayer.mediaPlayer.source = model.source
                                videoPlayer.videoId = model.video_id
                                videoPlayer.mediaPlayer.play()
                            }
                        }
                    }

                    Text {
                        id: videoTitle
                        text: model.video_name
                        width: parent.width - 5
                        height: 40
                        font.pointSize: 11
                        wrapMode: Text.WordWrap
                        fontSizeMode: Text.Fit
                        minimumPixelSize: 10
                        color: "#ffffff"

                        /*anchors.horizontalCenter: parent.horizontalCenter*/
                    }

                    RowLayout {
                        width: parent.width
                        height: 15

                        Text {
                            id: authorName
                            text: model.uploader + " · " + model.upload_date
                            width: parent.width - 5
                            height: 15
                            color: "#e9f1f6"
                            font.pointSize: 8
                        }

                        Button {
                            id: deleteVideoButton
                            text: "删除"
                            visible: editable ? true : false
                            Layout.alignment: Qt.AlignRight

                            onClicked: {
                                console.log("删除视频ID：" + model.video_id + " index: " + index)

                                videoController.deleteVideo(model.video_id, index)
                            }
                        }
                    }
                }
            }
        }

        ///////////////////////////////
    }

}
