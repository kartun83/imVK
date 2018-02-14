import bb.cascades 1.4
import VKChat 1.0

Page {
    id: communityCommPage
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
            dataModel: myVKChatController.wallCommentsModel
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    Container {
                        
                    Label {
                        text: ListItemData.fromID//Qt.myVKChatController.Qt.myVKChatController.getContactFullName( ListItemData.fromID )
                            textStyle.fontStyle: FontStyle.Default
                            textStyle.fontWeight: FontWeight.Bold

                        }
                    Label {
                        text: ListItemData.text
                        multiline: true
                    }
                        Divider {
//                            id: 
                        }
                    }                    
                }                
            ]
            onCreationCompleted: {
                Qt.myVKChatController = myVKChatController;
            }
        }
    }
}
