import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

Window {
    id: root
    width: 1400
    height: 800
    visible: true
    title: qsTr("Thinkthink")
    color: "#ee007400" // if alpha value is ff, fullscreen has a bug.
    flags: Qt.FramelessWindowHint


    Image {
        id: background
        fillMode: Image.TileHorizontally
        smooth: true
        source: "../images/1197280.jpg"
    }

    Image {
        id: logoImage
        source: "../images/Thinkthink.png"
        //width: 180
        x: 0
        y: 0
    }


    // Change mouse styles when the user moves it to the window edge.
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: {
            // console.info(mouse.x + " " + mouse.y)
            if((mouseX / width < 0.01 && mouseY / height < 0.01) || (mouseX / width > 0.99 && mouseY / height > 0.99)) cursorShape = Qt.SizeFDiagCursor // left top / right bottom
            else if((mouseX / width > 0.01 && mouseX / width < 0.99) && (mouseY / height < 0.01 || mouseY / height > 0.99)) cursorShape = Qt.SizeVerCursor // middle top / bottom
            else if((mouseX / width > 0.99 && mouseY / height < 0.01) || (mouseX / width < 0.01 && mouseY / height > 0.99)) cursorShape = Qt.SizeBDiagCursor // right top / left bottom
            else if((mouseX / width < 0.01 || mouseX / width > 0.99) && (mouseY / height > 0.01 && mouseY / height < 0.99)) cursorShape = Qt.SizeHorCursor // middle left / right
            else cursorShape = Qt.ArrowCursor
        }
        acceptedButtons: Qt.NoButton
    }

    // Response drag action and resize the window.
    DragHandler {
        id: resizeHandler
        grabPermissions: TapHandler.TakeOverForbidden
        target: null
        onActiveChanged: if (active) {
            const p = resizeHandler.centroid.position;
            let e = 0;
            if (p.x / width < 0.01) { e |= Qt.LeftEdge }
            if (p.x / width > 0.99) { e |= Qt.RightEdge }
            if (p.y / height < 0.01) { e |= Qt.TopEdge }
            if (p.y / height > 0.99) { e |= Qt.BottomEdge }
            root.startSystemResize(e);
        }
    }


    TitleBar {
        id: titleBar

        categoryList: categoryList
        videoLayout: videoLayout
        searchVideos: searchVideos
        myVideos: myVideos
        adminArea: adminArea
        userInfoArea: userInfoArea
    }


    CategoryList {
        id: categoryList
        width: 200
        anchors {
            top: titleBar.bottom
            left: parent.left
            bottom: parent.bottom
            topMargin: 10
        }

        videoLayout: videoLayout
    }

    VideoLayout {
        id: videoLayout
        anchors {
            top: titleBar.bottom
            left: categoryList.right
            right: parent.right
            bottom: parent.bottom
            topMargin: 10
            leftMargin: 40
            rightMargin: 40
        }

        gLoginWindow: titleBar.gLoginWindow
    }


    ///////////////////////////////////////////
    SearchVideos {
        id: searchVideos
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        visible: false

        categoryList: categoryList
        videoLayout: videoLayout
    }

    MyVideos {
        id: myVideos
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        visible: false

        categoryList: categoryList
        videoLayout: videoLayout
    }

    AdminArea {
        id: adminArea
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        visible: false

        categoryList: categoryList
        videoLayout: videoLayout
    }

    UserInfoArea {
        id: userInfoArea
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        visible: false

        categoryList: categoryList
        videoLayout: videoLayout
    }


}
