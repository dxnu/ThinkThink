import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    id: userInfoItem
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

            userInfoItem.visible = false
            categoryList.visible = true
            videoLayout.visible = true
        }

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 40
    }

    Rectangle {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            topMargin: 40
            leftMargin: 100
            rightMargin: 100
        }
        width: parent.width
        height: parent.height - 100
        opacity: 0.6
        color: "#d7dddb" // "#4f8a83"  "#e76278"

        ColumnLayout {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right

            Text {
                id: loginText
                text: "个人资料"
                font.pointSize: 20
                Layout.preferredWidth: 100
                Layout.preferredHeight: 50
                Layout.alignment: Qt.AlignCenter
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignCenter
                Layout.leftMargin: 100

                RowLayout {
                    Label {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 20
                        text: qsTr("账号:")
                    }
                    TextField {
                        id: accountIdText
                        placeholderText: "账号"
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 20
                        selectByMouse: true
                        readOnly: true
                    }
                }
                RowLayout {
                    Label {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 20
                        text: qsTr("昵称:")
                    }
                    TextField {
                        id: accountNameText
                        placeholderText: "账号"
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 20
                        selectByMouse: true
                    }
                }
                RowLayout {
                    Label {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 20
                        text: qsTr("性别:")
                    }
                    ComboBox {
                        id: genderCombox
                        model: ["男", "女", "保密"]
                        //textRole: "name"

                        Component.onCompleted: currentIndex = 0

                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 20
                    }
                }
                RowLayout {
                    Label {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 20
                        text: qsTr("邮箱:")
                    }
                    TextField {
                        id: mailText
                        placeholderText: "邮箱"
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 20
                        selectByMouse: true
                        readOnly: true
                    }
                }

                RowLayout {
                    Label {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 20
                        text: qsTr("注册日期:")
                    }
                    TextField {
                        id: dateText
                        placeholderText: "注册日期"
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 20
                        selectByMouse: true
                        readOnly: true
                    }
                }
                RowLayout {
                    Label {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 20
                        text: qsTr("修改签名:")
                    }
                    Rectangle {
                        id: commentArea
                        color: "#33e60012"
                        height: flick.height + 20
                        //width: flick.width
                        border.color: "#e60012"
                        border.width: 1
                        radius: 0.7
                        Layout.preferredWidth: 200


                        Flickable {
                             id: flick

                             width: parent.width; height: 100;
                             contentWidth: aboutMeTextEdit.paintedWidth
                             contentHeight: aboutMeTextEdit.paintedHeight
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
                                 id: aboutMeTextEdit
                                 width: flick.width
                                 color:"#f2fdff"
                                 focus: true
                                 selectByMouse: true
                                 wrapMode: TextEdit.Wrap
                                 onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
                             }
                         }
                    }
                }
                RowLayout {
                    Label {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 20
                        text: qsTr("头像:")
                    }
                    Image {
                        id: avatarImage
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 50
                    }
                }

                Button {
                    text: qsTr("保存")
                    Layout.preferredWidth: 250
                    Layout.preferredHeight: 40

                    onClicked: {
                        console.log(genderCombox.currentIndex)
                        accountController.account_name(accountNameText.text) // 昵称
                        accountController.gender(genderCombox.currentIndex)  // 性别
                        accountController.about_me(aboutMeTextEdit.text)     // 个性签名
                        accountController.updateInfo()
                    }
                }
            }
        }
    }



    Connections {
        target: accountController.model()
        function onInitUserInfo(user_id, user_name, mail, gender, about_me, avatar, date) {

            accountIdText.text = user_id
            accountNameText.text = user_name
            if(gender === "0") genderCombox.currentIndex = 0
            else if(gender === "1") genderCombox.currentIndex = 1
            else genderCombox.currentIndex = 2
            mailText.text = mail
            dateText.text = date
            aboutMeTextEdit.text = about_me
            avatarImage.source = avatar
        }
    }
}
