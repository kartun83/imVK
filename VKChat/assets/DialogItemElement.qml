import bb.cascades 1.4
import VKChat 1.0

//Page {
Container {
    accessibility.name: qsTr("Dialog element") + Retranslate.onLanguageChanged
    // Properties
//    property int titlebarHeight: 0
    property string description
    property string imageSource
    property string fullName    
    property string mesageText
    property alias messageTime : messageTimeLabel.text    
    property alias avatarImageViewData: avatarView.imageSource
    property alias hasAttachment : attachIcon.visible    
    property bool outMessage
    property bool online
    property bool readMessage
    /////////////
    horizontalAlignment: HorizontalAlignment.Fill
    attachedObjects: [
        LayoutUpdateHandler {
            id: titleBarLayout
            onLayoutFrameChanged: {
                // Tracking the titlebar height to guide size of avatar
                titlebarHeight = layoutFrame.height
            }
        }
    ]
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    background: ui.palette.background
    Container {
        verticalAlignment: VerticalAlignment.Center
        topPadding: ui.sdu(0.3)
        leftPadding: topPadding
        layout: DockLayout {
            
        }
        ImageView {
            id: avatarView
            objectName: "avatarView"
            accessibility.name: qsTr("Avatar view")
            preferredHeight: ui.px(100.0) 
            preferredWidth: preferredHeight*2 
            scalingMethod: ScalingMethod.AspectFill 
            layoutProperties: StackLayoutProperties {
                spaceQuota: -1
            }
//            imageSource: "asset:///img/icon_110.png"          
        }
        ImageView {
            id: onlineIndicator
            accessibility.name: qsTr("Online indicator")
            imageSource: online ? "asset:///img/online.png" : "asset:///img/offline.png" 
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Bottom
            preferredHeight: ui.sdu(2.0)
            preferredWidth: preferredHeight
            maxHeight: preferredHeight
            maxWidth: preferredHeight
            margin.rightOffset: ui.sdu(0.4)
            margin.bottomOffset: ui.sdu(0.2)
        }
        ImageView {
            id: splatIndicator
            accessibility.name: qsTr("Unread messages indicator")
            imageSource: "asset:///img/splat.png"
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Top
            preferredHeight: ui.sdu(3.0)
            preferredWidth: preferredHeight
            visible: readMessage
        }
    }
    Container // holds name & description
    {
        id: topContainer
        objectName: "topContainer"
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Center            
        implicitLayoutAnimationsEnabled: false
        visible: true
        //background: wantsHighlight ? _UIStyleManager.highlightBackgroundColor : undefined            
                
        Container {
            id: nameAndDescription
            objectName: "nameAndDescription"
            margin.rightOffset: ui.sdu(2.0)
            
            leftPadding:    ui.sdu(2.0)
            
            Container {
                id: nameContainer    
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                //topPadding:     ui.sdu(2.0)
                //                layoutProperties: StackLayoutProperties {
                //                    spaceQuota: -1
                //                }
                Label {
                    objectName: "name"
                    verticalAlignment: VerticalAlignment.Fill
                    content.flags: TextContentFlag.Emoticons | TextContentFlag.ActiveTextOff
                    leftMargin: 0
                    //text: qsTr("%1 %2").arg(firstName).arg(lastName)
//                    text: "Olga Tupikina"
                    text: fullName
                    textFormat: TextFormat.Plain
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    //textStyle.color: wantsHighlight ? _UIStyleManager.highlightTextColor : _UIStyleManager.titleBarPrimaryColor
                    textStyle.fontSize: FontSize.Medium
                    
                   layoutProperties: StackLayoutProperties {
                       spaceQuota: 1
                   }
                }    // Label for name
                Label {
                    accessibility.name: qsTr("Dialog item")
                    id: messageTimeLabel
                    horizontalAlignment: HorizontalAlignment.Right
//                    text: Utilities.fmtTime(ListItemData.date)
                }
            }
            
            Container {
//                background: Color.Blue
                horizontalAlignment: HorizontalAlignment.Fill
//                preferredWidth: ui.du(170.0)
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                id: descriptionContainer
                ImageView {
                    imageSource: outMessage == true ? "asset:///icons/ic_forward.amd" : ""
//                                                      "asset:///icons/ic_reply.amd"
                    filterColor: ui.palette.textOnPlain
                    maxHeight: messageTextLabel.maxHeight / 2
                    maxWidth: maxHeight
                    verticalAlignment: VerticalAlignment.Center
                    visible: outMessage
                }                            
                Label {
                    id: messageTextLabel
                    objectName: "MessageText"
                    verticalAlignment: VerticalAlignment.Center
                    content.flags: TextContentFlag.Emoticons | TextContentFlag.ActiveText                    
//                    text: description
//                    text: "Some long message"
                    text: unescape(mesageText)
                    textFormat: TextFormat.Auto
                    maxHeight: ui.du(5.0)
                    multiline: false
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        fontStyle: FontStyle.Italic
                        fontSize: FontSize.Small
                    }
                    
                    //textStyle.color: wantsHighlight ? _UIStyleManager.highlightTextColor : _UIStyleManager.titleBarSecondaryColor
                    
                    
                    // no horizontal fill or spaceQuota on purpose
                    // we want the label to be left aligned even with right-to-left text for consistency with the name label
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    // Label for description
                }
//                Label {
//                    horizontalAlignment: HorizontalAlignment.Right
//                    text: "attachment"
//                }
                ImageView {
                    id: attachIcon
                    accessibility.name: qsTr("Message have attachment indicator")
                    imageSource: "asset:///icons/ic_attach.amd"
                    maxHeight: messageTextLabel.maxHeight * 0.7
                    maxWidth: maxHeight
                    filterColor: ui.palette.textOnPlain
                }
            }       
        }
    }
    
//    Divider {
//        accessibility.name: qsTr("divider") + Retranslate.onLanguageChanged
//        topMargin: 0; bottomMargin: 0
//    }

}
//}
