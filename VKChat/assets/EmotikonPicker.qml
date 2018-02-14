import bb.cascades 1.4

//Page {
    Container {
        //id: emoticonsPicker
        preferredHeight: 0    // Hidden by default
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        attachedObjects: [
            ImplicitAnimationController {
                id: animList
                propertyName: "preferredHeight"
            }
        ]
    topPadding: ui.sdu(1.5)

    ListView {
            accessibility.name: qsTr("List of available emoticons")
            id: emoticon_list
            layout: GridListLayout {
            id: emoticon_layout
            columnCount: 15
            horizontalCellSpacing: ui.sdu(0.5)
            verticalCellSpacing: horizontalCellSpacing
//            orientation: LayoutOrientation.LeftToRight

        }
            dataModel: XmlDataModel {
                source: "asset:///data/emojies.xml"
//                source: "asset:///data/countries.xml"
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    
                    Container {
                        
                        Label {
                            leftPadding: 0
                            leftMargin: 0
                            content.flags: TextContentFlag.Emoticons | TextContentFlag.ActiveText
                            textFormat: TextFormat.Html
                            // Короткие в виде "&#x26C5;"
                            text: qsTr("&%1;").arg(ListItemData.name)
                            textStyle.fontSize: FontSize.XLarge
                            verticalAlignment: VerticalAlignment.Center                                            
                        }                        
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath);
                messageField.text = messageField.text  + " " + qsTr("&%1;").arg(chosenItem.name);
                //toogleEmoji();
            }   
            attachedObjects: [
                // This handler is tracking the layout frame of the button.
                LayoutUpdateHandler {
                    id: handler
                    onLayoutFrameChanged: {
                        // Individual layout frame values can be
                        // retrieved from the signal parameter
//                        console.log("Layout Frame: [" +
//                        layoutFrame.x + ", " + layoutFrame.y + ", " +
//                        layoutFrame.width + ", " + 
//                        layoutFrame.height + "]");
//                        console.log(emoticon_list.maxWidth);
//                        var res = qsTr("Setting columns:%1").arg(Math.floor(layoutFrame.width/85));
//                        console.log(res);                        
//                        emoticon_layout.setColumnCount(Math.floor(layoutFrame.width/ui.sdu(10.0)))
                        emoticon_layout.setColumnCount(Math.floor(layoutFrame.width/ui.sdu(9.5)))
                    }
                }
            ]
        scrollIndicatorMode: ScrollIndicatorMode.ProportionalBar
        flickMode: FlickMode.Default
    }
//    ListView {
//        preferredHeight: ui.du(8.0)
//        layout: StackListLayout {
//            orientation: LayoutOrientation.LeftToRight
//        }
//        dataModel: XmlDataModel {
//            source: "asset:///data/emojies.xml"
//            //                source: "asset:///data/countries.xml"
//            
//        }
//        listItemComponents: [
//            ListItemComponent {
//                type: "item"
//                
//                Container {
//                    leftMargin: ui.sdu(.0)
//                    Label {
//                        leftPadding: ui.sdu(1.0)
//                        leftMargin: ui.sdu(5.0)
//                        content.flags: TextContentFlag.Emoticons | TextContentFlag.ActiveText
//                        textFormat: TextFormat.Html
//                        // Короткие в виде "&#x26C5;"
//                        text: qsTr("&%1;").arg(ListItemData.name)
//                        textStyle.fontSize: FontSize.XLarge
//                        verticalAlignment: VerticalAlignment.Center                                            
//                    }                        
//                }
//            }
//        ]
//    }
 }
//}
