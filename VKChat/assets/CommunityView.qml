import bb.cascades 1.4
import bb.data 1.0
import VKChat 1.0
import "utilities.js" as Utilities

Page {
    id: communityPage
    objectName: "communityPage"
    titleBar: TitleBar
    {
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties
            {
                content: CustomTitleBannerControl 
                {
                    objectName: "banner"
                    //                contactName: qsTr("%1 %2").arg(myVKChatController.currentContact.first_name).arg(myVKChatController.currentContact.Last_name)
                    firstName: myVKChatController.currentCommunity.name
//                    lastName:  myVKChatController.currentCommunity.last_name
                    avatarImageViewData: myVKChatController.currentCommunity.photo_200
                    description: myVKChatController.currentCommunity.status
//                    online: myVKChatController.currentContact.online
                    showOnline : false
                    
                }
                onCreationCompleted: {
                    console.log("Name:"+myVKChatController.currentCommunity.name);
                    console.log("Status:"+myVKChatController.currentCommunity.status);
                    console.log(myVKChatController.currentCommunity);
                }
            }
    }
    Container {        
        ListView {
            dataModel: myVKChatController.wallModel
            
            onTriggered: {
                
                var _entry = dataModel.data(indexPath);
                var postID = _entry.postID;                    
                console.log("Displaying comments for:"+ postID); 
                myVKChatController.fetchWallComments(0, postID, true);                        
                var newPage = communityComments.createObject();
                commNaviPane.push(newPage); 
            }
            listItemComponents:[
                WallHeaderElement {
                    
                },
//                ListItemComponent {
//                    type: "header"
//                    Label {
//                        text: Utilities.fmtTime(ListItemData.date)
//                    }
//                },
                ListItemComponent {
                type: "item"
                
//                StandardListItem {
//                    title: ListItemData.text                 
//                }                
                Container {
                    contextActions: [
                        ActionSet {
                            ActionItem {
                                imageSource: "asset:///icons/ic_addLike.png"
                                title: qsTr("Like") + Retranslate.onLanguageChanged
                                enabled: ListItemData.canLike                                    
                            }
                            ActionItem {
                                imageSource: "asset:///icons/ic_forward.amd"
                                title: qsTr("Forward") + Retranslate.onLanguageChanged
                            }
                            ActionItem {
                                imageSource: "asset:///icons/ic_forward.amd"
                                title: qsTr("Repost") + Retranslate.onLanguageChanged
                                enabled: ListItemData.canPublish
                            }
                            ActionItem {
                                imageSource: "asset:///icons/ic_pin.png"
                                title: qsTr("Pin") + Retranslate.onLanguageChanged
                                enabled: ListItemData.canPin
                            }
                        }
                    ]
                                        
                    Label {
                        text: ListItemData.text
                        multiline: true                    
                    }     
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        ImageView {
                            imageSource: "asset:///icons/ic_liked.amd"
                            accessibility.name: qsTr("Likes count") + Retranslate.onLanguageChanged
                            filterColor: ui.palette.textOnPlain
                            preferredHeight: ui.sdu(2.5)
                            preferredWidth: preferredHeight
                            verticalAlignment: VerticalAlignment.Center 
                        }
                        Label {
                            leftMargin: ui.sdu(-1.0)
                            id: likesCountText
                            text: ListItemData.likesCount
                            multiline: false
                            verticalAlignment: VerticalAlignment.Center
                        }
                        ImageView {
                            accessibility.name: qsTr("Comments count") + Retranslate.onLanguageChanged
                            imageSource: "asset:///icons/ic_send.amd"
                            filterColor: ui.palette.textOnPlain
                            preferredHeight: ui.sdu(2.5)
                            preferredWidth: preferredHeight
                            verticalAlignment: VerticalAlignment.Center 
                        }
                        Label {
                            leftMargin: ui.sdu(-1.0)
                            id: commentsCountText
                            text: ListItemData.replyCount
                            multiline: false
                            verticalAlignment: VerticalAlignment.Center
                        }
                        
                        ImageView {
                            accessibility.name: qsTr("Repost count") + Retranslate.onLanguageChanged
                            imageSource: "asset:///icons/ic_forward.amd"
                            filterColor: ui.palette.textOnPlain
                            preferredHeight: ui.sdu(2.5)
                            preferredWidth: preferredHeight
                            verticalAlignment: VerticalAlignment.Center 
                        }
                        Label {
                            leftMargin: ui.sdu(-1.0)
                            id: repostCountText
                            text: ListItemData.repostCount
                            multiline: false
                            verticalAlignment: VerticalAlignment.Center
                        }                                                                    
                    }
                    Container {
                        background: ui.palette.textOnPlain
                        Divider {
                        }
                    }
            }           
            }
            ]
        }
//        layout: DockLayout {
//            
//        }
//        Label {
//            text: qsTr("Group view is currently under development")            
//            verticalAlignment: VerticalAlignment.Center
//            horizontalAlignment: HorizontalAlignment.Center
//            autoSize.maxLineCount: 3
//            textStyle.fontWeight: FontWeight.Default
//            textStyle.fontSize: FontSize.XLarge
//        }
    }
    actions: [
        ActionItem {
            imageSource: "asset:///icons/ic_compose.png"
            title: qsTr("Compose") + Retranslate.onLanguageChanged
            // TODO :: Check actual status :)
            enabled: true
            ActionBar.placement: ActionBarPlacement.Signature
            onTriggered: {
//                var _comm = dataModel.data(indexPath).id;                                    
                console.log("Opening community post sheet");                         
//                myVKChatController.setCurrentCommunityID(_comm);
                var newPage = communityPost.createObject();
                commNaviPane.push(newPage); 
            }

        }
    ]
}
