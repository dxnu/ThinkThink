import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs


Item {
    id: adminItem
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

            adminItem.visible = false
            categoryList.visible = true
            videoLayout.visible = true
        }

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 40
    }

    ListView {
        id: tabView
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            topMargin: 40
            leftMargin: 100
            rightMargin: 100
        }
        orientation: ListView.Horizontal
        height: 40
        clip: true
        interactive: false
        spacing: 8


        model: AdminButtonModel{}

        delegate: ItemDelegate {
            id: control
            implicitWidth: 60
            implicitHeight: 30


            contentItem: Item {
                Button {
                    id: tabButton
                    text: tabName

                    contentItem: Text {
                        text: parent.text
                        opacity: enabled ? 1.0 : 0.3
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }

                    background: Rectangle {
                        implicitWidth: control.implicitWidth
                        implicitHeight: control.implicitHeight
                        color: control.highlighted ? (control.hovered ? "#d7dddb" : "#712164")
                                                   : (control.hovered ? "#d7dddb" : "#e76278")
                        opacity: enabled ? 1 : 0.3
                        radius: 2
                    }

                    onClicked: {
                        tabView.forceActiveFocus()
                        tabView.currentIndex = model.index
                        if(model.tabName === "目录管理") {
                            //console.log("目录管理")
                            videoAdmin.visible = false
                            categoryAdmin.visible = true
                        } else if(model.tabName === "视频管理") {
                            //console.log("视频管理")
                            categoryAdmin.visible = false
                            videoAdmin.visible = true
                        }
                    }
                }
            }

            background: Rectangle {
                implicitWidth: parent.width
                implicitHeight: parent.height
                opacity: 0.6
                color: "#fac699"
            }

            highlighted: ListView.isCurrentItem
        }
    }

    function urlToPath(urlString) {
        var s
        if (urlString.startsWith("file:///")) {
            var k = urlString.charAt(9) === ':' ? 8 : 7
            s = urlString.substring(k)
        } else {
            s = urlString
        }

        return decodeURIComponent(s);
    }

    FileDialog {
        id: coverFileDialog
        title: "Please choose a file"

        property string type
        property int coverNumber
        property int categoryIndex
        property string sourceName
        onAccepted: {
            if(type === "image") {
                console.log(urlToPath(coverFileDialog.currentFile.toString()))

                // 先上传文件
                fileUploadController.setFilePath(urlToPath(coverFileDialog.currentFile.toString()))
                sourceName = fileUploadController.generateUniqueFileName()
                fileUploadController.uploadFile(sourceName)
                // 再更新数据库
                videoCategoryController.updateCategoryCover(coverNumber, categoryIndex, sourceName)
            }
        }
    }

    // 目录管理
    Item {
        id: categoryAdmin
        anchors.left: parent.left
        anchors.top: tabView.bottom
        anchors.right: parent.right
        anchors.leftMargin: 100
        anchors.rightMargin: 100
        width: parent.width
        height: parent.height

        // 目录管理
        ListView {
            id: categoryAdminListView
            width: parent.width
            height: parent.height - 150

            clip: true
            focus: true
            //interactive: false

            model: videoCategoryController.model()

            delegate: ItemDelegate {
                id: categoryAdminControl
                implicitWidth: categoryAdminListView.width
                implicitHeight:40
                text: name

                contentItem: Item {
                    Row {
                        spacing: 2

                        Text {
                            text: categoryAdminControl.text
                            font: categoryAdminControl.font
                            color: categoryAdminControl.enabled ? (categoryAdminControl.down ? "#f2fdff" : "#f2fdff") : "#f2fdff"
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                        }

                        Item {
                            width: categoryAdminControl.width - 250
                            height: 1
                        }

                        Button {
                            text: "封面一"
                            width: 50
                            height: 30
                            contentItem: Text {
                                text: parent.text
                                opacity: enabled ? 1.0 : 0.3
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                implicitWidth: parent.width
                                implicitHeight: parent.height
                                color: hovered ? "#d7dddb" : "#712164"
                                opacity: enabled ? 1 : 0.3
                                radius: 2
                            }

                            onClicked: {
                                coverFileDialog.type = "image"
                                coverFileDialog.coverNumber = 1
                                coverFileDialog.categoryIndex = index
                                coverFileDialog.nameFilters = ["Image files (*.jpg *.png)", "All files (*)"]
                                coverFileDialog.open()
                            }
                        }

                        Button {
                            text: "封面二"
                            width: 50
                            height: 30
                            contentItem: Text {
                                text: parent.text
                                opacity: enabled ? 1.0 : 0.3
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                implicitWidth: parent.width
                                implicitHeight: parent.height
                                color: hovered ? "#d7dddb" : "#712164"
                                opacity: enabled ? 1 : 0.3
                                radius: 2
                            }

                            onClicked: {
                                coverFileDialog.type = "image"
                                coverFileDialog.coverNumber = 2
                                coverFileDialog.categoryIndex = index
                                coverFileDialog.nameFilters = ["Image files (*.jpg *.png)", "All files (*)"]
                                coverFileDialog.open()
                            }
                        }

                        Button {
                            text: "封面三"
                            width: 50
                            height: 30
                            contentItem: Text {
                                text: parent.text
                                opacity: enabled ? 1.0 : 0.3
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                implicitWidth: parent.width
                                implicitHeight: parent.height
                                color: hovered ? "#d7dddb" : "#712164"
                                opacity: enabled ? 1 : 0.3
                                radius: 2
                            }

                            onClicked: {
                                coverFileDialog.type = "image"
                                coverFileDialog.coverNumber = 3
                                coverFileDialog.categoryIndex = index
                                coverFileDialog.nameFilters = ["Image files (*.jpg *.png)", "All files (*)"]
                                coverFileDialog.open()
                            }
                        }

                        Button {
                            text: "删除"
                            width: 50
                            height: 30
                            contentItem: Text {
                                text: parent.text
                                opacity: enabled ? 1.0 : 0.3
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                implicitWidth: parent.width
                                implicitHeight: parent.height
                                color: hovered ? "#d7dddb" : "#712164"
                                opacity: enabled ? 1 : 0.3
                                radius: 2
                            }

                            onClicked: {
                                console.log("删除目录ID：" + model.name + " index: " + index)
                                videoCategoryController.deleteCategory(model.name, index)
                            }
                        }
                    }
                }

                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    opacity: 0.6
                    color: categoryAdminControl.highlighted ? (categoryAdminControl.hovered ? "#d7dddb" : "#e76278")
                                                            : (categoryAdminControl.hovered ? "#d7dddb" : "#4f8a83")
                }

                highlighted: ListView.isCurrentItem

                onClicked: {
                    categoryAdminListView.forceActiveFocus()
                    categoryAdminListView.currentIndex = model.index
                }
            }
        }

        ScrollArea {
            orientation: Qt.Vertical
            height: categoryAdminListView.height;
            width: 8
            scrollArea: categoryAdminListView;
            anchors.right: categoryAdminListView.right
        }

        Row {
            anchors.top: categoryAdminListView.bottom
            height: 30

            TextField {
                id: addCategoryTextField
                width: 200
                height: parent.height
                placeholderText: qsTr("输入要增加的目录名吧~")
                selectByMouse: true
                verticalAlignment: TextInput.AlignVCenter

                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    color: "#d7dddb"
                    opacity: 0.3
                    radius: 2
                }
            }

            Button {
                id: addCategoryButton
                width: 50
                height: parent.height
                text: "添加"

                contentItem: Text {
                    text: parent.text
                    opacity: enabled ? 1.0 : 0.3
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    color: addCategoryButton.hovered ? "#d7dddb" : "#712164"
                    opacity: enabled ? 1 : 0.3
                    radius: 2
                }

                onClicked: {
                    if(addCategoryTextField.text.length !== 0){
                        console.log("添加目录" + addCategoryTextField.text)
                        videoCategoryController.insertCategory(addCategoryTextField.text)
                    }
                }
            }
        }
    }

    // 目录管理
    Item {
        id: videoAdmin
        anchors.left: parent.left
        anchors.top: tabView.bottom
        anchors.right: parent.right
        anchors.leftMargin: 100
        anchors.rightMargin: 100
        width: parent.width
        height: parent.height

        visible: false

        ListView {
            id: videoAdminListView
            width: parent.width
            height: parent.height - 150

            clip: true
            focus: true
            //interactive: false

            model: videoController.model()

            delegate: ItemDelegate {
                id: videoAdminControl
                implicitWidth: videoAdminListView.width
                implicitHeight:40
                //text: video_name

                contentItem: Item {
                    Row {
                        spacing: 2

                        Text {
                            id: videoNameText
                            width: 400
                            text: video_name
                            font: videoAdminControl.font
                            color: videoAdminControl.enabled ? (videoAdminControl.down ? "#f2fdff" : "#f2fdff") : "#f2fdff"
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                        }

                        Text {
                            id: videpUploaderText
                            width: 100
                            text: uploader
                            font: videoAdminControl.font
                            color: videoAdminControl.enabled ? (videoAdminControl.down ? "#f2fdff" : "#f2fdff") : "#f2fdff"
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                        }

                        Text {
                            id: uploadDateText
                            width: 300
                            text: upload_date
                            font: videoAdminControl.font
                            color: videoAdminControl.enabled ? (videoAdminControl.down ? "#f2fdff" : "#f2fdff") : "#f2fdff"
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.Wrap
                        }

                        Item {
                            width: videoAdminControl.width - videoNameText.width - videpUploaderText.width - uploadDateText.width - 100
                            height: 1
                        }

                        Button {
                            text: "删除"
                            width: 50
                            height: 30
                            contentItem: Text {
                                text: parent.text
                                opacity: enabled ? 1.0 : 0.3
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                implicitWidth: parent.width
                                implicitHeight: parent.height
                                color: hovered ? "#d7dddb" : "#712164"
                                opacity: enabled ? 1 : 0.3
                                radius: 2
                            }

                            onClicked: {
                                console.log("删除视频ID：" + model.video_id + " index: " + index)
                                videoController.deleteVideo(model.video_id, index)
                            }
                        }
                    }
                }

                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    opacity: 0.6
                    color: videoAdminControl.highlighted ? (videoAdminControl.hovered ? "#d7dddb" : "#e76278")
                                                            : (videoAdminControl.hovered ? "#d7dddb" : "#4f8a83")
                }

                highlighted: ListView.isCurrentItem

                onClicked: {
                    videoAdminListView.forceActiveFocus()
                    videoAdminListView.currentIndex = model.index
                }
            }
        }

        ScrollArea {
            orientation: Qt.Vertical
            height: videoAdminListView.height;
            width: 8
            scrollArea: videoAdminListView;
            anchors.right: videoAdminListView.right
        }

    }

}
