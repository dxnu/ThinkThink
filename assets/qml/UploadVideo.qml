import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


Window {
    id: uploadWindow
    width: 400
    height: 340
    title: qsTr("Thinkthink")

    property string userId

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

    onClosing: {
        uploadVideoErrorTips.visible = false
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"

        property string type
        onAccepted: {
            if(type === "image")
                videoCoverPath.text = urlToPath(fileDialog.currentFile.toString())
            else if(type === "video")
                videoSourcePath.text = urlToPath(fileDialog.currentFile.toString())
        }
    }


    // 视频上传界面
    ColumnLayout {
        id: loginColumn
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        Text {
            id: loginText
            text: "上传视频"
            font.pointSize: 20
            Layout.preferredWidth: 100
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignCenter
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                id: uploadVideoErrorTips
                Layout.preferredWidth: 250
                Layout.preferredHeight: 20
                Layout.alignment: Qt.AlignCenter
                color: "red"
                radius: 5
                visible: false

                Text {
                    id: uploadVideoErrorText
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("标题:")
                }
                TextField {
                    id: videoTitle
                    placeholderText: "视频标题"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("封面图片:")
                }
                TextField {
                    id: videoCoverPath
                    placeholderText: "视频封面"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                    readOnly: true
                }

                Button {
                    text: qsTr("...")
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 20

                    onClicked: {
                        fileDialog.type = "image"
                        fileDialog.nameFilters = ["Image files (*.jpg *.png)", "All files (*)"]
                        fileDialog.open()
                    }
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("视频文件:")
                }
                TextField {
                    id: videoSourcePath
                    placeholderText: "视频文件"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                    readOnly: true
                }

                Button {
                    text: qsTr("...")
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 20

                    onClicked: {
                        fileDialog.type = "video"
                        fileDialog.nameFilters = ["Video files (*.mp4)", "All files (*)"]
                        fileDialog.open()
                    }
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("视频简介:")
                }
                TextField {
                    id: videoDescribe
                    placeholderText: "用一段简短的话来描述视频吧~"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("视频分类:")
                }
//                TextField {
//                    id: videoCategory
//                    placeholderText: "视频分类"
//                    Layout.preferredWidth: 200
//                    Layout.preferredHeight: 20
//                }
                ComboBox {
                    id: videoCategory
                    model: videoCategoryController.model()
                    textRole: "name"

                    Component.onCompleted: currentIndex = 0

                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                }
            }

            Button {
                Layout.preferredWidth: 250
                Layout.alignment: Qt.AlignCenter
                text: qsTr("上传视频")

                property string coverName
                property string sourceName

                onClicked: {
                    uploadVideoErrorTips.visible = false

                    if(videoTitle.text.length == 0) {
                        uploadVideoErrorTips.visible = true;
                        uploadVideoErrorText.text = "视频标题不能为空"
                        return;
                    }

                    if(videoCoverPath.text.length == 0) {
                        uploadVideoErrorTips.visible = true;
                        uploadVideoErrorText.text = "没有选择视频封面"
                        return;
                    }

                    if(videoSourcePath.text.length == 0) {
                        uploadVideoErrorTips.visible = true;
                        uploadVideoErrorText.text = "没有选择视频文件"
                        return;
                    }
                    if(videoCategory.currentText.length == 0) {
                        uploadVideoErrorTips.visible = true;
                        uploadVideoErrorText.text = "没有选择视频分类"
                        return;
                    }

                    console.log(videoCoverPath.text)
                    console.log(videoSourcePath.text)


                    // 上传视频封面
                    fileUploadController.setFilePath(videoCoverPath.text)
                    coverName = fileUploadController.generateUniqueFileName()
                    fileUploadController.uploadFile(coverName)

                    // 上传视频文件
                    fileUploadController.setFilePath(videoSourcePath.text)
                    sourceName = fileUploadController.generateUniqueFileName()
                    fileUploadController.uploadFile(sourceName)

                    videoController.uploadVideo(videoTitle.text, userId, coverName, sourceName, videoCategory.currentText, videoDescribe.text)
                }
            }
        }
    }

    Connections {
        target: videoController.model()
        function onUploadVideoReply(status, errorMsg) {
            if(!status) {
                uploadVideoErrorTips.visible = true
                uploadVideoErrorText.text = errorMsg
            }
            else {
                uploadVideoErrorTips.visible = true
                uploadVideoErrorText.text = "上传成功"

                videoCategoryController.updateCategories()

                videoTitle.text = "";
                videoCoverPath.text = "";
                videoSourcePath.text = "";
                videoDescribe.text = "";
                videoCategory.currentIndex = 0;
            }
        }

    }
}
