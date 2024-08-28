import QtQuick
import QtQuick.Controls


Item {

    property bool showHotVideoImage: true
    property bool itemEditable: false
    property string itemShowText
    property LoginWindow gLoginWindow

    property StackView stackView:stackView

    // 展示区域
    // 有背景的版本 其中implicitHeight供ScrollView推测出contentHeight
    ScrollView {
        id: videoLayoutScroll
        width: parent.width
        height: parent.height

        // 设置滚动条样式
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical: ScrollBar {
            parent: videoLayoutScroll.parent
            anchors.top: videoLayoutScroll.top
            anchors.left: videoLayoutScroll.right
            anchors.bottom: videoLayoutScroll.bottom
            height: videoLayoutScroll.availableHeight
        }

        clip: true

        Rectangle {
            width: parent.width
            implicitHeight:videoLayoutColumn.height
            color: "#994f8a83" // "#41555d"

            Column {
                id: videoLayoutColumn
                width: videoLayoutScroll.width
                spacing: 10

//                Image {
//                    id: defaultImage
//                    asynchronous: true
//                    cache: true
//                    source: "../images/220302.png"
//                }

                StackView {
                    id: stackView
                    width: parent.width
                    height: Math.ceil(parent.width / 4)
                    //initialItem: defaultImage

                    replaceEnter: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 0
                            to: 1
                            duration: 1500
                        }
                    }
                    replaceExit: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 1
                            to: 0
                            duration: 500
                        }
                    }

                    visible: showHotVideoImage ? true : false
                }

                ////////////////////////////////////

                VideoItem {
                    editable: itemEditable
                    showText: itemShowText

                    loginWindow: gLoginWindow
                }

                ///////////////////////////////////
            }
        }
    }

//    ScrollView {
//        id: videoLayoutScroll
//        width: parent.width
//        height: parent.height

//        // 设置滚动条样式
//        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
//        ScrollBar.vertical: ScrollBar {
//            parent: videoLayoutScroll.parent
//            anchors.top: videoLayoutScroll.top
//            anchors.left: videoLayoutScroll.right
//            anchors.bottom: videoLayoutScroll.bottom
//            height: videoLayoutScroll.availableHeight
//        }

//        clip: true

//        // 展示区域
//        // 无背景的版本
//        Column {
//            width: videoLayoutScroll.width
//            spacing: 10

////            Rectangle {
////                width: parent.width
////                height: 100
////                color: "red"
////            }

//            Image {
//                id: hotVideoImage
//                width: parent.width
//                height: Math.ceil(parent.width / 4)
//                source: "images/220302.png"
//            }

////            Rectangle {
////                width: parent.width
////                height: 100
////                color: "green"
////            }
//            ////////////////////////////////////

//            VideoItem {
//                // width: parent.width
//            }

//            ///////////////////////////////////
//        }
//    }
}
