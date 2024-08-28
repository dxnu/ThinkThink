import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: loginWindow
    width: 400
    height: 340
    title: qsTr("Thinkthink")

    property Image profileImage
    property Text userNameText
    property string userId
    property bool loggedIn:false

    // 登录界面
    ColumnLayout {
        id: loginColumn
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        Text {
            id: loginText
            text: "账户登录"
            font.pointSize: 20
            Layout.preferredWidth: 100
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignCenter
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                id: loginErrorTips
                Layout.preferredWidth: 250
                Layout.preferredHeight: 20
                Layout.alignment: Qt.AlignCenter
                color: "red"
                radius: 5
                visible: false

                Text {
                    id: loginErrorText
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("用户名:")
                }
                TextField {
                    id: loginAccountId
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
                    text: qsTr("密   码:")
                }
                TextField {
                    id: loginPassword
                    placeholderText: "密码"
                    echoMode: TextInput.Password
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                    selectByMouse: true
                }
            }
        }


        RowLayout {
            Layout.alignment: Qt.AlignCenter

            Button {
                text: "忘记密码"

                onClicked: {
                    loginColumn.visible = false
                    loginAccountId.text = ""
                    loginPassword.text = ""
                    resetPasswordColumn.visible = true
                }
            }
            Button {
                text: "注册账户"

                onClicked: {
                    loginColumn.visible = false
                    signupColumn.visible = true
                }
            }
        }

        Button {
            Layout.preferredWidth: 250
            Layout.alignment: Qt.AlignCenter
            text: "登录"

            onClicked: {
                loginErrorTips.visible = false;

                if(loginAccountId.text.length == 0) {
                    loginErrorText.text = "用户名不能为空"
                    loginErrorTips.visible = true
                    return
                }

                if(loginPassword.text.length == 0) {
                    loginErrorText.text = "密码不能为空"
                    loginErrorTips.visible = true
                    return
                }

                accountController.login(loginAccountId.text, loginPassword.text)
            }
        }
    }

    // 注册界面
    ColumnLayout {
        id: signupColumn
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        visible: false

        property string verificationCode

        Text {
            id: signupText
            text: "账户注册"
            font.pointSize: 20
            Layout.preferredWidth: 100
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignCenter
        }

        Rectangle {
            id: errorTips
            Layout.preferredWidth: 250
            Layout.preferredHeight: 20
            Layout.alignment: Qt.AlignCenter
            color: "red"
            radius: 5
            visible: false

            Text {
                id: errorText
                Layout.fillWidth: true
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("用户名:")
                }
                TextField {
                    id: registerAccountId
                    placeholderText: "限数字/字母/_，不能_或数字开头"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                }
            }
            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("密   码:")
                }
                TextField {
                    id: registerPassword
                    placeholderText: "密码"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                    echoMode: TextInput.Password
                }
            }
            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("确认密码:")
                }
                TextField {
                    id: registerRepassword
                    placeholderText: "确认密码"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                    echoMode: TextInput.Password
                }
            }
            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("昵   称:")
                }
                TextField {
                    id: registerAccountName
                    placeholderText: "昵称"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                }
            }
            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("签   名:")
                }
                TextField {
                    id: registerAboutMe
                    placeholderText: "签名"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                }
            }
            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("邮   箱:")
                }
                TextField {
                    id: registerMail
                    placeholderText: "注册后不可修改"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                }
            }
            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("验证码:")
                }
                TextField {
                    id: registerVerifyCode
                    placeholderText: "输入验证码"
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 20
                }
                Button {
                    Layout.preferredWidth: 80
                    text: "获取验证码"

                    onClicked: {
                        // 此处要检查邮箱合法性
                        errorTips.visible = false
                        if(!accountController.checkMail(registerMail.text)) {
                            errorText.text = "请输入有效的邮箱"
                            errorTips.visible = true
                            return
                        }
                        accountController.getVerifyCode(registerMail.text)
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignCenter

                Button {
                    text: "登录账户"

                    onClicked: {
                        signupColumn.visible = false
                        loginColumn.visible = true
                    }
                }
            }

            Button {
                Layout.preferredWidth: 250
                Layout.alignment: Qt.AlignCenter
                text: "注册"

                onClicked: {
                    errorTips.visible = false

                    if(registerAccountId.text.length == 0) {
                        errorText.text = "用户名不能为空"
                        errorTips.visible = true
                        return
                    }

                    if(registerPassword.text.length == 0) {
                        errorText.text == "密码不能为空"
                        errorTips.visible = true
                        return
                    }

                    if(registerPassword.text !== registerRepassword.text) {
                        errorText.text = "密码不一致"
                        errorTips.visible = true
                        return
                    }

                    if(registerAccountName.text.length == 0) {
                        errorText.text = "用户名昵称不能为空"
                        errorTips.visible = true
                        return
                    }

                    if(!accountController.checkMail(registerMail.text)) {
                        errorText.text = "请输入有效的邮箱"
                        errorTips.visible = true
                        return
                    }

                    if(registerVerifyCode.text.length == 0) {
                        errorText.text = "请输入验证码"
                        errorTips.visible = true
                        return
                    }

                    if(registerVerifyCode.text !== signupColumn.verificationCode) {
                        errorText.text = "验证码错误"
                        errorTips.visible = true
                        return
                    }

                    accountController.account_id(registerAccountId.text)
                    accountController.account_name(registerAccountName.text)
                    accountController.password(registerPassword.text)
                    accountController.mail(registerMail.text)
                    accountController.about_me(registerAboutMe.text)
                    accountController.registerAccount()
                }
            }
        }
    }

    Connections {
        target: accountController.model()
        function onRegisterAccountReply(status, errorMsg) {
            if(!status) {
                errorTips.visible = true
                errorText.text = errorMsg
            }
            else {
                signupColumn.visible = false
                loginColumn.visible = true

                registerAccountId.text = ""
                registerPassword.text = ""
                registerRepassword.text = ""
                registerAccountName.text = ""
                registerMail.text = ""
                registerVerifyCode.text = ""
                registerAboutMe.text = ""
            }
        }
        function onChangePasswordReply(status, errorMsg) {
            if(!status) {
                resetPasswordErrorTips.visible = true
                resetPasswordErrorText.text = errorMsg
            }
            else {
                resetPasswordColumn.visible = false
                loginColumn.visible = true

                resetPasswordAccount.text = ""
                resetPasswordAccountPassword.text = ""
                resetPasswordRepassword.text = ""
                resetPasswordVerifyCode.text = ""
            }
        }
        function onVerificationCodeChanged(code) {
            signupColumn.verificationCode = code
        }
        function onResetPasswordVerificationCodeChanged(code) {
            resetPasswordColumn.resetPasswordVerificationCode = code
        }
        function onLoginResponse(status, message, name, image) {
            if(!status) {
                loginErrorTips.visible = true
                loginErrorText.text = message
            }
            else {
                loggedIn = true
                loginWindow.visible = false
                userId = loginAccountId.text
                userNameText.text = name
                profileImage.source = image

                // 保存当前用户ID
                accountController.logged(true)
                accountController.account_id(loginAccountId.text)
                accountController.account_name(name)
                accountController.profile_photo(image)
            }
        }
    }

    ColumnLayout {
        id: resetPasswordColumn
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        visible: false

        property string resetPasswordVerificationCode

        Text {
            id: resetPasswordText
            text: "重置密码"
            font.pointSize: 20
            Layout.preferredWidth: 100
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignCenter
        }

        Rectangle {
            id: resetPasswordErrorTips
            Layout.preferredWidth: 250
            Layout.preferredHeight: 20
            Layout.alignment: Qt.AlignCenter
            color: "red"
            radius: 5
            visible: false

            Text {
                id: resetPasswordErrorText
                Layout.fillWidth: true
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("账     号:")
                }
                TextField {
                    id: resetPasswordAccount
                    placeholderText: "输入账号"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("新 密 码:")
                }
                TextField {
                    id: resetPasswordAccountPassword
                    placeholderText: "输入新密码"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                    echoMode: TextInput.Password
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("确认密码:")
                }
                TextField {
                    id: resetPasswordRepassword
                    placeholderText: "请再次输入密码"
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 20
                    echoMode: TextInput.Password
                }
            }

            RowLayout {
                Label {
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 20
                    text: qsTr("验证码:")
                }
                TextField {
                    id: resetPasswordVerifyCode
                    placeholderText: "输入验证码"
                    Layout.preferredWidth: 120
                    Layout.preferredHeight: 20
                }
                Button {
                    Layout.preferredWidth: 80
                    text: "获取验证码"

                    onClicked: {
                        resetPasswordErrorTips.visible = false

                        if(resetPasswordAccount.text.length == 0) {
                            resetPasswordErrorText.text = "请输入用户名"
                            resetPasswordErrorTips.visible = true
                            return
                        }

                        accountController.getResetPasswordVerifyCode(resetPasswordAccount.text)
                    }
                }
            }
        }

        Button {
            Layout.preferredWidth: 250
            Layout.alignment: Qt.AlignCenter
            text: "修改密码"

            onClicked: {
                resetPasswordErrorTips.visible = false

                if(resetPasswordAccount.text.length == 0) {
                    resetPasswordErrorText.text = "用户名不能为空"
                    resetPasswordErrorTips.visible = true
                    return
                }

                if(resetPasswordAccountPassword.text.length == 0) {
                    resetPasswordErrorText.text == "密码不能为空"
                    resetPasswordErrorTips.visible = true
                    return
                }

                if(resetPasswordAccountPassword.text !== resetPasswordRepassword.text) {
                    resetPasswordErrorText.text = "密码不一致"
                    resetPasswordErrorTips.visible = true
                    return
                }

                if(resetPasswordVerifyCode.text.length == 0) {
                    resetPasswordErrorText.text = "请输入验证码"
                    resetPasswordErrorTips.visible = true
                    return
                }

                if(resetPasswordVerifyCode.text !== resetPasswordColumn.resetPasswordVerificationCode) {
                    resetPasswordErrorText.text = "验证码错误" + resetPasswordColumn.resetPasswordVerificationCode
                    resetPasswordErrorTips.visible = true
                    return
                }

                console.log("change password---")
                accountController.account_id(resetPasswordAccount.text)
                accountController.password(resetPasswordAccountPassword.text)
                accountController.changePassword()
            }
        }

    }

}
