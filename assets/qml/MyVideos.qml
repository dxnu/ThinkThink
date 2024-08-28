import QtQuick
import QtQuick.Controls

Item {
    id: myVideos
    property CategoryList categoryList
    property VideoLayout videoLayout

    RoundButton {
        id: backspaceButton
        width: 50
        height: 30
        background: Text {
            text: qsTr("\u2039")
            font.pointSize: 20
            color: backspaceButton.hovered ? "#0488d0" : "#000000"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        onClicked: {
            videoController.updateVideos(videoCategoryController.at(0))

            myVideos.visible = false
            categoryList.visible = true
            videoLayout.visible = true
        }

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 40
    }

    VideoLayout {
        id: myVideosReulst
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: 40
            leftMargin: 100
            rightMargin: 100
        }
        showHotVideoImage: false
        itemEditable: true
        itemShowText: "我的视频"
    }
}
