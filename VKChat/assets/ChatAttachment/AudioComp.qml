import bb.cascades 1.4
import "../utilities.js" as Utilities

Container {
    rightPadding: ui.sdu(1.5)
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
//    Label {
//        text: "Audio"
//    }
    ImageView {
        accessibility.name: qsTr("Audio icon")
        imageSource: "asset:///icons/ic_doctype_music.amd"
        filterColor: ui.palette.textOnPlain
        maxHeight: ui.du(4.0)
        maxWidth: maxHeight
        minWidth: maxWidth
        verticalAlignment: VerticalAlignment.Center
    }
    Label {
        id: contentLabel
//        text: qsTr("Audio attachments currently not supported") //ListItemData.body
        text: qsTr("%1 - %2").arg(ListItemData.artist.toString().trim()).arg(ListItemData.title.toString().trim())
//        text: qsTr("%1 - %2").arg("Edwin feat maestro ACID best track ever !11!!!11111111!!!11!!111").arg("Выключи меня")
        multiline: false
        onCreationCompleted: {
            console.log("AUDIO URL:" + ListItemData.body);
        }
        autoSize.maxLineCount: -1
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
    }
    Label {            
        multiline: false
        text: Utilities.fmtDuration(ListItemData.duration);
        layoutProperties: StackLayoutProperties {
            spaceQuota: -1
        }
        horizontalAlignment: HorizontalAlignment.Right
    }
    onTouch: {
        if (event.isUp())
        {
            Qt.myVKChatController.play(ListItemData.body, ListItemData.artist.toString().trim(), ListItemData.title.toString().trim());
        }
    }
}
