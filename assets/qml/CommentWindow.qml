import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Window {
    width: 600
    height: 400
    title: qsTr("评论")
    color: "#50616d"

    property string videoId

    property LoginWindow gLoginWindow


    Image {
        id: background
        fillMode: Image.TileHorizontally
        smooth: true
        source: "../images/1197280.jpg"
    }

    Label {
        id: commentLabel
        text: qsTr("评论")
        font.pointSize: 14
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 10
        color: "#ffffff"
    }

    Button {
        id: userAvatarButton
        anchors.left: parent.left
        anchors.top: commentLabel.bottom
        anchors.leftMargin: 10
        anchors.topMargin: 5

        height: 30
        width: 30
        background: Image {
            id: avatarImage
            source: "http://127.0.0.1/default/avatar3.jpg"
        }
    }

    Rectangle {
        id: commentArea
        color: "#33e60012"
        height: flick.height + 20
        //width: flick.width
        border.color: "#e60012"
        border.width: 1
        radius: 0.7
        anchors.left: userAvatarButton.right
        anchors.top: commentLabel.bottom
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.leftMargin: 5
        anchors.topMargin: 5


        Flickable {
             id: flick

             width: parent.width; height: 100;
             contentWidth: commentTextEdit.paintedWidth
             contentHeight: commentTextEdit.paintedHeight
             clip: true


             function ensureVisible(r)
             {
                 if (contentX >= r.x)
                     contentX = r.x;
                 else if (contentX+width <= r.x+r.width)
                     contentX = r.x+r.width-width;
                 if (contentY >= r.y)
                     contentY = r.y;
                 else if (contentY+height <= r.y+r.height)
                     contentY = r.y+r.height-height;
             }

             TextEdit {
                 id: commentTextEdit
                 width: flick.width
                 color:"#f2fdff"
                 focus: true
                 selectByMouse: true
                 wrapMode: TextEdit.Wrap
                 onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
             }
         }

        Button {
            text: qsTr("发布")
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            onClicked: {
                if(!accountController.logged()) {
                    console.log("请先登录")
                    loginWindow.modality = Qt.WindowModal
                    loginWindow.show()
                    return;
                }

                // 上传评论
                videoCommentController.uploadComment(accountController.getUserId(), videoId, commentTextEdit.text)
                commentTextEdit.text = ""
            }
        }
    }

    ListView {
        id: comments
        anchors.left: parent.left
        anchors.top: commentArea.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        clip: true

        model: videoCommentController.model()

        delegate: ItemDelegate {
            id: control
            implicitWidth: comments.width
            implicitHeight: commentItem.height

            contentItem: Item {
                id: commentItem
                height: avatarButton.height + commentItemColumn.height

                Button {
                    id: avatarButton
                    height: 30
                    width: 30
                    background: Image {
                        source: model.user_avatar_source
                    }
                }

                Column {
                    id: commentItemColumn
                    anchors.left: avatarButton.right
                    anchors.leftMargin: 5

                    RowLayout {
                        width: commentItem.width - 50
                        Text {
                            text: model.user_name
                            color:"#f2fdff"
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                        }
                        Text {
                            Layout.alignment: Qt.AlignRight
                            text: model.time
                            color: "#f2fdff"
                        }
                    }

                    Item {
                        width: 1 // dummy value != 0, an item of width 0 won't work
                        height: 5
                    }

                    Text {
                        text:  model.content
                        color: "#f2fdff"
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.Wrap
                    }
                }
            }

            background: Rectangle {
                id: listViewBackground
                implicitWidth: parent.width
                implicitHeight: parent.height
                opacity: 0.6
                color: "#758a99"
            }

            highlighted: ListView.isCurrentItem

            onClicked: {
                comments.forceActiveFocus()
                comments.currentIndex = model.index
            }
        }
    }

    ScrollArea {
        id: listScrollBar

        orientation: Qt.Vertical
        anchors.top: comments.top
        height: comments.height;
        width: 8
        scrollArea: comments;
        anchors.right: comments.right
    }

    Connections {
        target: videoCommentController.model()

        function onUploadVideoCommentsReply(status, errorMsg) {
            if(status) {
                videoCommentController.getComments(videoId)
            } else {
                console.log("发布评论失败")
            }
        }
    }

}
