import bb.cascades 1.4

Container {
//    background: Color.Red    
//    property alias content : contentLabel.text
//    Label {
//        text: "Image. ContentType:" + ListItemData.contentType
//    }
    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Center   
    ImageView {
        accessibility.name: "AttachmentImage"
        accessibility.description: qsTr("Attahcment image") + Retranslate.onLanguageChanged
        imageSource: ListItemData.body 
        attachedObjects: [
            Invocation {
                id: invoke
                query {
                    mimeType: "image"                    
                    uri: ListItemData.body
                    invokeActionId: "bb.action.VIEW"
                    invokeTargetId: "sys.pictures.card.previewer"
                    onQueryChanged: updateQuery();
                }
            }
//            QueryResultSheet {}
        ]
        
        onTouch: {
            if (event.isUp())
            {           
                console.log("Opening image viewer with: " + invoke.query.uri);     
                invoke.trigger("bb.action.VIEW");
            }
        }
        contextActions: [
            ActionSet {                        
                // General SHARE Framework call
                // Will display all the SHARE Targets available
                InvokeActionItem {
                    ActionBar.placement: ActionBarPlacement.InOverflow
                    query {
                        mimeType: "image/jpeg"
                        invokeActionId: "bb.action.SHARE"
                        uri: ListItemData.body
                    }
                    onTriggered: {
//                        Qt.myVKChatController.invoke(invokeTarget, "bb.action.SHARE", "image/jpeg", ListItemData.body);
                        data = ListItemData.body;
                        
                        console.log("Sharing:"+data);
                    }
                }
            }
        ]
    }
}
