import bb.cascades 1.4
import bb.data 1.0
import VKChat 1.0
import "utilities.js" as Utilities

Page {
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
        topPadding: ui.sdu(2.0)
        leftPadding: topPadding
        rightPadding: leftPadding
        bottomPadding: topPadding
        TextArea {
            id: newPostContent
            maximumLength: -1
            textFormat: TextFormat.Auto
            inputMode: TextAreaInputMode.Text
            input.submitKey: SubmitKey.Submit
            input.submitKeyFocusBehavior: SubmitKeyFocusBehavior.Default
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            layoutProperties: StackLayoutProperties {

            }

        }
        attachedObjects: [
            // This handler is tracking the layout frame of the button.
            LayoutUpdateHandler {
                id: handler
                onLayoutFrameChanged: {
                    newPostContent.preferredHeight = layoutFrame.height;
//                    newPostContent.minHeight = layoutFrame.height;
                }
            }
        ]
    }
    
    actions: [
        ActionItem {
            imageSource: "asset:///icons/ic_send.amd"
            ActionBar.placement: ActionBarPlacement.Signature
            title: qsTr("Submit") + Retranslate.onLanguageChanged
            onTriggered: {
                myVKChatController.postToCurrentCommunity(newPostContent.text);
                commNaviPane.pop();
            }
        }
    ]
}
