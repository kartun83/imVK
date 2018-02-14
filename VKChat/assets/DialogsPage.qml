import bb.cascades 1.4
import bb.system 1.2
import bb.data 1.0
import "utilities.js" as Utilities


NavigationPane {
    id: diaNaviPane
    Page {
        
        id: dialogsPage
        Container {
            id: dialogsContainer
            //        _media_text  = 0,
            //        _media_photo = 1,
            //        _media_doc   = 2,
            //        _media_video = 3,
            //        _media_audio = 4,
            //        _media_sticker = 6,
            //        _media_wall   = 5,
            //        _media_wall_reply = 7,
            property variant attachmentNames: [ 'text', 'photo', 'document', 'video', 'audio', 'wall', 'sticker', 'wall reply']
            onCreationCompleted: {
//                attachmentNames.push(qsTr("text"), qsTr("photo"), qsTr("document"), qsTr("video"), qsTr("audio"), 
//                                     qsTr("wall"), qsTr("sticker"), qsTr("wall reply"));
                Qt.attachmentTypes = dialogsContainer;
            }
            ListView {                
                id: dialogsList
                dataModel: myVKChatController.dialogsModel                
                accessibility.description: qsTr("List of dialogs");
                accessibility.name: qsTr("Dialogs")
                listItemComponents: ListItemComponent {
                type: "item"
                CustomListItem {
                    accessibility.name: qsTr("Dialog item")
                    dividerVisible: true
                    highlightAppearance: HighlightAppearance.Frame
                    
                    DialogItemElement {
                        property int contactID : ListItemData.out == true ? ListItemData.user_id : ListItemData.from_id                        
                        fullName:  //Qt.myVKChatController.getContactFullName( contactID ) 
                                                             ListItemData.out == true ? Qt.myVKChatController.getContactFullName( contactID ) :
                                                             Qt.myVKChatController.getContactFullName( contactID )
//                        qsTr("out:%1 , fromid: %2, %userid: %3").arg(ListItemData.out, ListItemData.from_id, ListItemData.user_id)
//                                    ListItemData.user_id
//                        imageSource: 
                         
                        outMessage: ListItemData.out
                        readMessage : ListItemData.read_state ==  0 ? true : false
                        messageTime: Utilities.fmtTime(ListItemData.date)
                        mesageText: ListItemData.body != "" ? ListItemData.body : Qt.attachmentTypes.attachmentNames[ListItemData.lastAttachmentType]  
                        hasAttachment: ListItemData.hasAttachment
                        avatarImageViewData: Qt.myVKChatController.getContact(contactID).photo_100
                        online: Qt.myVKChatController.getContact(contactID).online
                    }
                }
//                    StandardListItem {
//                        title: Qt.myVKChatController.getContactFullName( ListItemData.user_id )
//                        imageSource: ListItemData.photo_100
//                        //description: qsTr("%1").arg(fmtTime(ListItemData.last_seen.time))   
//                        status: Utilities.fmtTime(ListItemData.date)                
//                        description: ListItemData.body
////                        property int contactID : ListItemData.contactID;                 
//                    }
                }
                onTriggered: {                                
                    var _message = dataModel.data(indexPath);
                    var contactID = _message.out ? _message.user_id : _message.from_id;                    
                    console.log("Starting dialog with:"+ contactID);                         
                    myVKChatController.setCurrentContactID(contactID);
                    var newPage = chatDefinition.createObject();
                    diaNaviPane.push(newPage); 
                }
                onCreationCompleted: {
                    Qt.myVKChatController = myVKChatController;
                }
                snapMode: SnapMode.LeadingEdge
                flickMode: FlickMode.Momentum
                bufferedScrollingEnabled: true
                stickToEdgePolicy: ListViewStickToEdgePolicy.BeginningThenEnd
            }    // ListView
        } // Container
//        actions: [
//            ActionItem {
//                title: qsTr("Reload") + Retranslate.onLanguageChanged
//                imageSource: "asset:///icons/ic_reload.png"
//            },
//            ActionItem {
//                
//            }        
//        ]
    } // Page
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
    attachedObjects: [
        ComponentDefinition {
            id: chatDefinition
            source: "Chat.qml"
        }
    ]
}