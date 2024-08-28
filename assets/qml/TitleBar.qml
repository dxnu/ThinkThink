import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls


Item {
    property CategoryList categoryList
    property VideoLayout videoLayout
    property SearchVideos searchVideos
    property MyVideos myVideos
    property AdminArea adminArea
    property UserInfoArea userInfoArea
    property LoginWindow gLoginWindow:loginWindow


    width: parent.width
    height: 50

    anchors {
        top: parent.top
        right: parent.right
        left: parent.left
        margins: 5, 0, 5, 5
    }


    DragHandler {
        target: null
        onActiveChanged: if(active) { root.startSystemMove() }
    }

    LoginWindow {
        id: loginWindow
        profileImage: avatarImage
        userNameText: userName
    }

    UploadVideo {
        id: uploadVideoWindow
    }

    RowLayout {
        id: titleBarLayout
        anchors.bottom: parent.bottom
        width: parent.width
        height: 30


        // Search Box
        Rectangle {
            id: searchBox
            width: 300
            height: parent.height
            color: "#88808080"
            opacity: 0.5
//            border.color: "#000000"
//            border.width: 1
            radius: 50
            Layout.leftMargin: 240

            RowLayout {
                spacing: 2
                anchors.fill: parent

                RoundButton {
                    id: searchButton
                    implicitWidth: 30
                    implicitHeight: parent.height

                    background: Text {
                        text: qsTr("\u2315")
                        font.pixelSize: Qt.application.font.pixelSize * 1.8
                        color: searchButton.hovered ? "#0488d0" : "#000000"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        // search video
                        if(searchTextInput.text.length == 0) {
                            searchTextInput.text = "请输入关键字，进行搜索"
                            return
                        }
                        videoController.searchVideos(searchTextInput.text)

                        categoryList.visible = false
                        videoLayout.visible = false
                        searchVideos.visible = true
                    }
                }

                TextInput {
                    id: searchTextInput
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    selectByMouse: true
                    opacity: 0.5

                    onFocusChanged: {
                        if(activeFocus) {
                            searchBox.width = 400
                            // there should add animation
                        } else {
                            searchBox.width = 300
                        }
                    }
                }
            }
        }


        Row {
            Button {
                id: userAvatarButton
                height: 30
                width: 30
                background: Image {
                    id: avatarImage
                    source: "http://127.0.0.1/default/avatar3.jpg"
                }
                onClicked: {
                    if(!loginWindow.loggedIn) {
                        loginWindow.modality = Qt.WindowModal
                        loginWindow.show()
                    }
                }
            }

            Text {
                id: userName
                text: qsTr("未登录")
                font.pixelSize: 14
                color: "red"
            }
        }

        Popup {
            id: funcAreaPopup
            x: funcAreaButton.x
            y: funcAreaButton.y + funcAreaButton.height
            width: 100
            height: 150
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

            ColumnLayout {
                anchors.fill: parent
                Button {
                    text: qsTr("上传视频")
                    Layout.preferredWidth: parent.width

                    onClicked: {
                        if(!loginWindow.loggedIn) {
                            loginWindow.modality = Qt.WindowModal
                            loginWindow.show()
                        } else {
                            uploadVideoWindow.userId = loginWindow.userId
                            uploadVideoWindow.modality = Qt.WindowModal
                            uploadVideoWindow.show()
                        }
                        funcAreaPopup.close()
                    }
                }
                Button {
                    text: qsTr("我的视频")
                    Layout.preferredWidth: parent.width

                    onClicked: {
                        if(loginWindow.loggedIn) {
                            // 获取当前用户的所有视频
                            videoController.getMyVideos(loginWindow.userId)

                            categoryList.visible = false
                            videoLayout.visible = false
                            myVideos.visible = true
                        } else {
                            loginWindow.modality = Qt.WindowModal
                            loginWindow.show()
                        }

                        funcAreaPopup.close()
                    }
                }

                Button {
                    text: qsTr("资料设置")
                    Layout.preferredWidth: parent.width

                    onClicked: {

                        if(loginWindow.loggedIn) {
                            categoryList.visible = false
                            videoLayout.visible = false
                            userInfoArea.visible = true
                        } else {
                            loginWindow.modality = Qt.WindowModal
                            loginWindow.show()
                        }

                        funcAreaPopup.close()
                    }
                }

                Button {
                    id: adminButton
                    text: qsTr("后台管理")
                    Layout.preferredWidth: parent.width

                    onClicked: {
                        categoryList.visible = false
                        videoLayout.visible = false
                        adminArea.visible = true

                        funcAreaPopup.close()

                        // 此处获取所有视频
                        videoController.updateVideos("ALL")
                    }
                }

                Button {
                    text: qsTr("退出登录")
                    Layout.preferredWidth: parent.width

                    onClicked: {
                        if(loginWindow.loggedIn) {
                            loginWindow.loggedIn = false
                            avatarImage.source = "http://127.0.0.1/default/avatar3.jpg"
                            userName.text = qsTr("未登录")

                            if(myVideos.visible) {
                                videoController.updateVideos(videoCategoryController.at(0))

                                myVideos.visible = false
                                categoryList.visible = true
                                videoLayout.visible = true
                            }
                        }
                        funcAreaPopup.close()
                    }
                }
            }
        }



        RoundButton {
            id: funcAreaButton
            Layout.preferredWidth: 40
            implicitHeight: titleBarLayout.height
            onClicked: {
                if(loginWindow.userId !== "admin")
                    adminButton.visible = false
                else
                    adminButton.visible = true

                funcAreaPopup.open()
            }

            background: Text {
                text: qsTr("\u2261")
                font.pixelSize: Qt.application.font.pixelSize * 1.8
                color: funcAreaButton.hovered ? "#0488d0" : "#000000"
            }
        }


        // Minimum, Maximum, Close, etc.
        RowLayout {
            spacing: 0
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 5

            RoundButton {
                id: minimumButton
                implicitWidth: 30
                implicitHeight: titleBarLayout.height
                onClicked: root.showMinimized()

                background: Text {
                    text: qsTr("\u2013")
                    font.pixelSize: Qt.application.font.pixelSize * 1.8
                    color: minimumButton.hovered ? "#0488d0" : "#000000"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            RoundButton {
                id: maximumButton
                implicitWidth: 30
                implicitHeight: titleBarLayout.height
                onClicked: root.visibility === Window.Maximized ? root.showNormal() : root.showMaximized()

                background: Text {
                    text: root.visibility === Window.Maximized ? qsTr("🗗") : qsTr("\u25a1")
                    font.pixelSize: Qt.application.font.pixelSize * 1.8
                    color: maximumButton.hovered ? "#0488d0" : "#000000"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            RoundButton {
                id: closeButton
                implicitWidth: 30
                implicitHeight: titleBarLayout.height
                onClicked: root.close()

                background: Text {
                    text: qsTr("\u2a2f")
                    font.pixelSize: Qt.application.font.pixelSize * 1.8
                    color: closeButton.hovered ? "#0488d0" : "#000000"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
