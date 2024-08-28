import QtQuick
import QtQuick.Controls


FocusScope {
    id: root

    //CategoryFeeds { id: categoryFeeds }

    property VideoLayout videoLayout
    property var imgList: [img1, img2, img3]
    property string img1_url
    property string img2_url
    property string img3_url

    Image {
        id: img1
        visible: false
        source: img1_url
    }
    Image {
        id: img2
        visible: false
        source: img2_url
    }
    Image {
        id: img3
        visible: false
        source: img3_url
    }

    Timer {
        id: coverTimer
        interval: 3000
        repeat: true
        running: true
        triggeredOnStart: true
        property int count:0
        onTriggered: {
//            var i = categories.currentIndex
//            var j = count++ % 3
//            console.log("index=" + i + " cover:" + videoCategoryController.getCoverSource(i, j + 1))

            //videoLayout.coverImage = videoCategoryController.getCoverSource(i, j + 1)
            videoLayout.stackView.replace(imgList[count++ % 3])
        }
    }

    ListView {
        id: categories
        width: parent.width
        height: parent.height
        keyNavigationWraps: true
        clip: true
        focus: true

        model: videoCategoryController.model()

        delegate: ItemDelegate {
            id: control
            implicitWidth: categories.width
            implicitHeight:40
            text: name

            contentItem: Text {
                text: control.text
                font: control.font
                color: control.enabled ? (control.down ? "#f2fdff" : "#f2fdff") : "#f2fdff"
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.Wrap
            }

            background: Rectangle {
                id: listViewBackground
                implicitWidth: parent.width
                implicitHeight: parent.height
                opacity: 0.6
                color: control.highlighted ? (control.hovered ? "#50616d" : "#3d3b4f")
                                           : (control.hovered ? "#50616d" : "#758a99")
            }

            highlighted: ListView.isCurrentItem

            onClicked: {
                categories.forceActiveFocus()
                categories.currentIndex = model.index
                videoController.updateVideos(model.name)

                //videoLayout.coverImage = dummyImage.source //"http://127.0.0.1/video/227638136733300.jpg"

                update_cover_url(model.cover1_source, model.cover2_source, model.cover3_source)


            }
        }

    }


    ScrollArea {
        id: listScrollBar

        orientation: Qt.Vertical
        height: categories.height;
        width: 8
        scrollArea: categories;
        anchors.right: categories.right
    }

    function update_cover_url(cover1, cover2, cover3) {
        console.log("cover1: " + cover1)
        console.log("cover2: " + cover2)
        console.log("cover3: " + cover3)

        img1_url = cover1
        img2_url = cover2
        img3_url = cover3
    }

    Connections {
        target: videoCategoryController.model()
        function onCategoryChanged(cover1, cover2, cover3) {
            //console.log("category changed" + cover)
            update_cover_url(cover1, cover2, cover3)
        }
    }

}
