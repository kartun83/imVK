import bb.cascades 1.4
import bb.system 1.2
import bb.data 1.0


NavigationPane {
//    Menu.definition: MenuDefinition {
//        // Add a Help action
//        helpAction: HelpActionItem {
//            title: qsTr("About")
//            imageSource: "asset:///icons/ic_info.png"
//        }
//        
//        // Add a Settings action
//        settingsAction: SettingsActionItem {
//            title: qsTr("Settings")
//            imageSource: "asset:///icons/ic_settings.png"
//        }            
//    }
    id: contactsNaviPane
    //backButtonsVisible: false
    attachedObjects: [
        ComponentDefinition {
            id: chatDefinition
            source: "Chat.qml"
        }
    ]
Page {
    
    id: contactsPage
    objectName: "contactsPage"
//    function timeConverter(UNIX_timestamp){
//        var a = new Date(UNIX_timestamp*1000);
//        var months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
//        var year = a.getFullYear();
//        var month = months[a.getMonth()];
//        var date = a.getDate();
//        var hour = a.getHours();
//        var min = a.getMinutes();
//        var sec = a.getSeconds();
//        var time = date + ',' + month + ' ' + year + ' ' + hour + ':' + min + ':' + sec ;
//        return time;
//    }
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
                showOnline : false    // Don't show our online status in title, it's meaningless
                showBanner : true 
            }
        }
    }
    Container {
        ListView {
        id: contactsList
        dataModel: myVKChatController.contactsModel
        accessibility.name: qsTr("Contacts list")
    
        listItemComponents: ListItemComponent {
        type: "item"
    
                StandardListItem {
                    title: ListItemData.first_name + " " + ListItemData.last_name
                    imageSource: ListItemData.photo_100
                    //description: qsTr("%1").arg(fmtTime(ListItemData.last_seen.time))                   
                    description: ListItemData.status
                    property int contactID : ListItemData.contactID;                 
                }
            }
            onTriggered: {                                
                var _contact = dataModel.data(indexPath);                
                console.log(_contact.contactID);                         
                myVKChatController.setCurrentContactID(_contact.contactID);
                var newPage = chatDefinition.createObject();
                contactsNaviPane.push(newPage); 
            }            
            
//            contextActions: ActionSet {
//                
//                ActionItem {
//                    title: qsTr("Contact info");
//                    //imageSource: "asset:///icons/ic_add.png"
//                    onTriggered: {
//                        var newPage = contactInfoDefinition.createObject();
//                        navigationPane.push(newPage); 
//                    }
//                }
//                
//                DeleteActionItem {                                
//                    title: qsTr("Delete");
//                }
//            }
        }
    }
//    actions: [
//        ActionItem {
//             id: actionSortAlpha
//             title: qsTr("Alphabetically")
//             imageSource: "asset:///icons/ic_sort.png"
//             ActionBar.placement: ActionBarPlacement.InOverflow
//             onTriggered: {
//                 myVKChatController.sortContacts(0);
//             }
//        },
//        ActionItem {
//            id: actionSortOnline
//            title: qsTr("Online")
//            imageSource: "asset:///icons/ic_sort.png"
//            ActionBar.placement: ActionBarPlacement.InOverflow
//            onTriggered: {
//                myVKChatController.sortContacts(1);
//            }
//        },
//        ActionItem {
//            id: actionSortBanned
//            title: qsTr("Banned")
//            imageSource: "asset:///icons/ic_clear.png"
//            ActionBar.placement: ActionBarPlacement.InOverflow
//            onTriggered: {
//                myVKChatController.sortContacts(2);
//            }
//        }
//    ]
}
    onPopTransitionEnded: 
    {         
        console.log("Destroying:" + page.objectName);
        switch (page.objectName)
        {
            case "chatPage" : myVKChatController.unsetCurrentContact();
        }        
        Application.menuEnabled = true;
        page.destroy(); 
    }
}