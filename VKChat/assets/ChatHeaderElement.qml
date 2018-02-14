import bb.cascades 1.4
import "utilities.js" as Utilities

//Page {
ListItemComponent {    
    
    type: "header"
//    property alias contactName : contact_name.text

    Container {
        id: top_level  
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Center        
        topPadding: ui.du(1.5) 
        //        bottomPadding: bottomPadding 
        property bool out
        rightPadding: ListItemData.out ? ui.du(6.0) : ui.du(1.5) 
        leftPadding: rightPadding == ui.du(6.0) ? ui.du(1.5) : ui.du(6.0)
        //        bottomMargin: ui.du(1.5)
        Container {
            id: contentContainer
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            rightPadding: ui.du(2.0)
            leftPadding: rightPadding
            background: ui.palette.plainBase  
            Label {
                id: contact_name
                //                text: "Olga"
                textStyle.fontWeight: FontWeight.Bold
                // TODO :: Make proper, add full name accessor directly to contact
                text: ListItemData.out ? Qt.myVKChatController.getContactFullName( Qt.myVKChatController.myInfo.contactID ) 
                                       : Qt.myVKChatController.getContactFullName( ListItemData.from_id )
                //                             Qt.myVKChatController.getContactFullName( ListItemData.from_id )
                horizontalAlignment: HorizontalAlignment.Left
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 5
                }
                onCreationCompleted: {
                    // TODO :: Move to proper model to avoid this shit
//                    contact_name.text == " " ? contact_name.visible = false : contact_name.visible = true;
                }
            }            
            Label {
                id: message_time
                text:  Utilities.fmtTime(ListItemData.date)
                //                text: "19.31"
                horizontalAlignment: HorizontalAlignment.Right
                visible: contact_name.visible
            }
            horizontalAlignment: HorizontalAlignment.Fill
        }
        
    }
    
}
