import bb.cascades 1.4

//Page {
    Container {          
        property alias body : bodyLabel.text     
        property alias when : whenLabel.text
        property alias avatar : banner.avatarImageViewData
        property alias firstName : banner.firstName
        property alias lastName  : banner.lastName
        property alias status    : banner.description
        property alias online    : banner.online
        layout: DockLayout {
//            orientation: LayoutOrientation.TopToBottom
        }
        verticalAlignment: VerticalAlignment.Fill        
        background: ui.palette.background
        
        CustomTitleBannerControl 
         {
             id: banner
             objectName: "banner"
             showOnline: whenLabel.text == "" ? false : true 
             //                contactName: qsTr("%1 %2").arg(myVKChatController.currentContact.first_name).arg(myVKChatController.currentContact.Last_name)
//             firstName: myVKChatController.currentContact.first_name
//             lastName:  myVKChatController.currentContact.last_name
//             avatarImageViewData: myVKChatController.currentContact.photo_100
 //            description: myVKChatController.currentContact.status
//             firstName: "Alexey"
//             lastName:  "Tveritinov"
//             avatarImageViewData: "asset:///img/ava_50.jpg"
//             description: "Loving you"
             showBanner : false
             verticalAlignment: VerticalAlignment.Top
         }
        
//        Divider {
//            accessibility.name: "divider"
//        }
        Container {
              maxHeight: ui.sdu(2.0)
              preferredHeight: maxHeight
              minHeight: maxHeight
//              background: Color.Red
              verticalAlignment: VerticalAlignment.Top
        }
        Label {
            id: bodyLabel
            multiline: true
            topMargin: ui.sdu(1.5)
            accessibility.name: qsTr("Message")
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Fill
            autoSize.maxLineCount: 5
            textFormat: TextFormat.Plain            
            content.flags: TextContentFlag.Emoticons | TextContentFlag.ActiveText
            textStyle.color: ui.palette.textOnPlain
        //            text: "Message"            
        }
        Label {
            accessibility.name: qsTr("Message time")
            id: whenLabel
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Right
            textStyle.color: ui.palette.textOnPlain
//            text: "1 hour ago"
        }       
        
//        onCreationCompleted: {
//            myVKChatController.latestMessageUpdated.connect(onLatestMessageUpdated);            
//        } 
//        
//        function onLatestMessageUpdated()
//        {
//            var abc = myVKChatController.latestMessage
//            console.log(abc);
//            console.log(bodyLabel.text);
//            console.log(body);
//        }
    }
//}
