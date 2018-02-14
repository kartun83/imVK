import bb.cascades 1.4
import "utilities.js" as Utilities

import QTLoader 1.0

/**
 * The following import statement is needed for declaring internal Component elements.
 * Unfortunately the Component element is exported into the QtQuick namespace even though
 * it doesn't depend on QtQuick in any way.
 */
import QtQuick 1.0 as QtBase

//Page {
ListItemComponent {    
    
    type: "item"
//    property alias contactName : contact_name.text    
    Container {
        contextActions: [
            ActionSet {  
                ActionItem {
                    title: qsTr("Forward") + Retranslate.onLanguageChanged
                    imageSource: "asset:///icons/ic_forward.amd"  
                }
//                ActionItem {
//                    title: qsTr("Mark as important") + Retranslate.onLanguageChanged
//                    imageSource: "asset:///icons/ic"
//                }
                DeleteActionItem {
                    title: qsTr("Delete") + Retranslate.onLanguageChanged
                }   
            }
        ]
        id: top_level  
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Center        
 
        property bool out
        rightPadding: ListItemData.out ? ui.du(6.0) : ui.du(1.5) 
        leftPadding: rightPadding == ui.du(6.0) ? ui.du(1.5) : ui.du(6.0)
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            background: ui.palette.plainBase
            preferredWidth: top_level.maxWidth
//            background: Color.Red
            rightPadding: ui.du(2.0)
            leftPadding: top_level.leftPadding
            Loader {
                horizontalAlignment: HorizontalAlignment.Fill
                
                // Update the source of the loader depending on the position of the slider
                source: ListItemData.contentType == 100 ? "ChatAttachment/"+Qt.topLevel.pageNames[0]:"ChatAttachment/"+Qt.topLevel.pageNames[ListItemData.contentType]
            
            }
//            Label {
//                text: ListItemData.body
////                text: "Some text"
//                id: bodyLabel
//                multiline: true
//                textStyle.color: ui.palette.textOnPlain
//                content.flags: TextContentFlag.ActiveText | TextContentFlag.Emoticons
//                textFormat: TextFormat.Auto
//                                
//            }                        

        }
//        onCreationCompleted: {
//            var model = top_level.parent.dataModel.data([top_level.ListItem.indexPath,0]);
//            console.log("Model:"+model.toString()); 
//        }
    }
}
