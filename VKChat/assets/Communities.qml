import bb.cascades 1.4
import VKChat 1.0

NavigationPane {
    id: commNaviPane        
    
    Page {
        titleBar: TitleBar
        {            
            kind: TitleBarKind.FreeForm
            kindProperties: FreeFormTitleBarKindProperties
            {
                content: CustomTitleBannerControl 
                {
                    firstName: myVKChatController.myInfo.first_name
                    lastName : myVKChatController.myInfo.last_name
                    avatarImageViewData: myVKChatController.myInfo.photo_100
                    description: myVKChatController.myInfo.status
//                    online: false
                    showOnline : false    // Don't show our online status in title, it's meaningless
                }
            }
        }
        
        Container {
            ListView {
                accessibility.name: qsTr("List of communities")
                dataModel: myVKChatController.communitiesModel
                id: commListView
                stickToEdgePolicy: ListViewStickToEdgePolicy.BeginningThenEnd
                listItemComponents: ListItemComponent {
                    type: "item"
                    
                    StandardListItem {
                        title: ListItemData.name
                        imageSource: ListItemData.photo_200
                        //description: qsTr("%1").arg(fmtTime(ListItemData.last_seen.time))                   
                        description: ListItemData.status                                        
                    }
                }
                onTriggered: {                                
                    var _comm = dataModel.data(indexPath).id;                                    
                    console.log("Opening community:"+ _comm);                         
                    myVKChatController.setCurrentCommunityID(_comm);
                    var newPage = communityDefinition.createObject();
                    commNaviPane.push(newPage); 
                }
                onCreationCompleted: {
                    Qt.myVKChatController = myVKChatController;
                }
            }
        }
        
//        actions: ActionItem {
//            title: qsTr("Second page")
//            ActionBar.placement: ActionBarPlacement.OnBar
//            
//            onTriggered: {
//                navigationPane.push(secondPageDefinition.createObject());
//            }
//        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: communityDefinition
            source: "CommunityView.qml"
        },
        ComponentDefinition {
            id: communityPost
            source: "CommunityCompose.qml"
        },
        ComponentDefinition {
            id: communityComments
            source: "CommunityComments.qml"
        }
    ]
    
    onPopTransitionEnded: {
        
         console.log("Destroying:" + page.objectName);
         switch (page.objectName)
         {
             case "communityPage" : myVKChatController.unsetCommunity();
         }            
         Application.menuEnabled = true;            
         page.destroy();
         
    }
}
