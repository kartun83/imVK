import bb.cascades 1.4

//Page {
    Container {
        property alias sender : whoLabel.text
        property alias body   : bodyLabel.text
        property alias when   : whenLabel.text
        layout: DockLayout {
            //            orientation: LayoutOrientation.TopToBottom
        }

        background: ui.palette.background

        leftPadding: ui.sdu(2.0)
        topPadding: ui.sdu(2.0)
        bottomPadding: ui.sdu(2.0)
        Label {
            id: whoLabel
            accessibility.name: qsTr("Concact name")
            multiline: false
//            text: "Olga"
            margin.leftOffset: ui.sdu(2.0)
            leftMargin: 30.0

        }
        Label {
            accessibility.name: qsTr("Message")
            margin.topOffset: ui.sdu(5.0)
            id: bodyLabel
            multiline: true
//            text: "somethig"
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Fill
            autoSize.maxLineCount: 3
            //            text: "Message"
        }
        Label {
            accessibility.name: qsTr("Message time")
//            text: "1 day ago"
            id: whenLabel
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Fill
            //            text: "1 hour ago"
        }
    }
//}
