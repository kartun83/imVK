import bb.cascades 1.4
import "../utilities.js" as Utilities

Container {
    rightPadding: ui.sdu(1.5)
    leftPadding:  ui.sdu(1.0)
    layout: StackLayout {
        orientation: LayoutOrientation.TopToBottom
    }

    Container {        
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        //    Label {
        //        text: "Audio"
        //    }
        ImageView {
            accessibility.name: qsTr("Audio icon")
            imageSource: "asset:///icons/ic_doctype_video.amd"
            filterColor: ui.palette.textOnPlain
            maxHeight: ui.du(4.0)
            maxWidth: maxHeight
            minWidth: maxWidth
            verticalAlignment: VerticalAlignment.Center
        }
        Label {
            id: contentLabel
            //        text: qsTr("Audio attachments currently not supported") //ListItemData.body
            //        text: qsTr("%1 - %2").arg(ListItemData.artist.toString().trim()).arg(ListItemData.title.toString().trim())
    //        text: ListItemData.title
            text: ListItemData.title //"Каземир Малевич - освобождённое ничто"
            multiline: false
            onCreationCompleted: {
                console.log("Video URL:" + ListItemData.body);
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
    } // End of header container
    ImageView {
        imageSource: ListItemData.body
    }
}