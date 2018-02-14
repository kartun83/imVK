import bb.cascades 1.4

Page {
    id: chatPage
    objectName: "chatPage"
    Label {
        text: "Test message"
    }
//    property variant smileyPage
//    property string smileyToAdd
//    property int emoticonHeight
////    actionBarAutoHideBehavior: ActionBarAutoHideBehavior.HideOnScroll
////    actionBarVisibility: ChromeVisibility.Compact
//    
//    onCreationCompleted: {
//        myVKChatController.chatUpdated.connect(scrollToBottom);          
//    }    
//    function scrollToBottom(){
//        chatListView.scrollToPosition(ScrollPosition.End, ScrollAnimation.Smooth);
//        console.log("Received message, scrolling to bottom");
//    }
//    
//    function sendMessage(){
//        myVKChatController.sendMessageToCurrentContact(messageField.text.replace(/<[^>]*>/g, ""));
//        messageField.text = "";        
//    }
//    
//    function toogleEmoji() {
//        if(emoticonsPicker.preferredHeight == 0) {
//            emoticonsPicker.preferredHeight=300;
//            messageField.preferredHeight=ui.sdu(4);
//            myVKChatController.hideVirtualKeyboard();
//            scrollToBottom();
//        } else {
//            emoticonsPicker.preferredHeight=0;
//            messageField.preferredHeight=ui.sdu(15);
//        }
//    }
//    
//    titleBar: TitleBar
//    {
//        kind: TitleBarKind.FreeForm
//        kindProperties: FreeFormTitleBarKindProperties
//        {
//            content: CustomTitleBannerControl 
//            {
//                objectName: "banner"
////                contactName: qsTr("%1 %2").arg(myVKChatController.currentContact.first_name).arg(myVKChatController.currentContact.Last_name)
//                firstName: myVKChatController.currentContact.first_name
//                lastName:  myVKChatController.currentContact.last_name
//                avatarImageViewData: myVKChatController.currentContact.photo_100
//                description: myVKChatController.currentContact.status
//                online: myVKChatController.currentContact.online
//                showOnline : true
//                showBanner : true
//                //rightButtonIcon: _UIAssetManager.bannerNewChatIcon
//                //rightButtonIconPressed: _UIAssetManager.bannerNewChatPressedIcon
//                //rightButtonIconDisabled: _UIAssetManager.bannerNewChatDisabledIcon
//                //: Accessibility - translate this for screen reader to be brief, most important information first even if not grammatically correct. https://wikis.rim.net/display/bbxdev/BB10+Screen+Reader#BB10ScreenReader-GuidingPrinciples
//                //: \n the button on the top right of chats tab to start a new chat
//                //rightButtonAccessibilityName: qsTr("New Chat") + Retranslate.onLanguageChanged
//                
////                onRightButtonClicked: {
////                    startChatAction.triggered();
////                }
////                
////                onBannerTapped: {
////                    editProfile();
////                }
//            }
//        }
//    }
//    Container {
//        id: contentContainer
//
//        property variant pageNames: ['BodyComponent.qml', 'ImageComp.qml', 'DocComp.qml', 'VideoComp.qml', 'AudioComp.qml', 
//                                     'WallComp.qml', 'ImageComp.qml', 'WallComp.qml' ]
//        onCreationCompleted: {
//            Qt.topLevel = contentContainer;
//        }
//        layout: StackLayout {
//
//        }
//
//        ListView {
////            connections {
////                target: myVKChatController
////                onchatUpdated2: chatListView.scrollToPosition(ScrollPosition.End, ScrollAnimation.Smooth);
////            }
//            
//            id: chatListView
//            dataModel: myVKChatController.chatModel2
//
//            //            dataModel: myVKChatController.chatModel
//            accessibility.name: qsTr("Messages_list")
//            accessibility.description: "Messages list"
//            listItemComponents: [
//                ChatHeaderElement { },
//                ChatItemElement { }
//            ]
//            onCreationCompleted: {
//                Qt.myVKChatController = myVKChatController;
//            }
//            
//            //                ListItemComponent {
//            //              type: "item"
//            //
//            //              StandardListItem {
//            //                  title: qsTr("%1: %2").arg(ListItemData.from_id).arg(ListItemData.body)
//            //                  //imageSource: ListItemData.photo_50
//            //                  //description: qsTr("%1").arg(fmtTime(ListItemData.last_seen.time))
//            //                  description: ListItemData.date
//            //              }
//            // }
////            onSelectionChanged: {
////                //myProjectManager.setCurrentProject(indexPath);
////                console.log(indexPath);
////            }            
//
//            bufferedScrollingEnabled: true            
//        }
//        Container {
//            topPadding: ui.sdu(2.0)
//            layout: StackLayout {
//                orientation: LayoutOrientation.TopToBottom
//            }        
//        Container {
//            id: inputContainer
//            layout: StackLayout {
//                orientation: LayoutOrientation.LeftToRight
//
//            }            
//
//            ImageView {
//                id: smile_button
//                imageSource: "asset:///icons/smile.amd"
////                imageSource: "asset:///icons/keyboard6.amd" 
//                loadEffect: ImageViewLoadEffect.FadeZoom
//                accessibility.description: "Smile button"
//                accessibility.name: "Smile button"
//                preferredWidth: 64.0
//                preferredHeight: 64.0
//                verticalAlignment: VerticalAlignment.Center
//                horizontalAlignment: HorizontalAlignment.Center
//                margin.leftOffset: ui.sdu(2.0)
//                filterColor: ui.palette.textOnPlain                
//                onTouch: {
//                    if (event.isUp())
//                    {
//                        toogleEmoji();    
//                    }
//                }
//            }
//            TextArea {
//                id: messageField
//                inputMode: TextAreaInputMode.Chat
//                horizontalAlignment: HorizontalAlignment.Fill
//                accessibility.name: "Chat_input_field"
//                accessibility.description: "Chat input field"
//                textFormat: TextFormat.Html
//                    minHeight: ui.sdu(3.0)
//                    maxHeight: ui.sdu(15.0)
//                    input {
//                    submitKey: SubmitKey.Send   
//                    keyLayout: KeyLayout.Text
//                    flags: TextContentFlag.Emoticons | TextContentFlag.ActiveText
//                    onSubmitted: {
//                        sendMessage();
//                        scrollToBottom();
//                    }
//                     
//                }
//                    
//                 onFocusedChanged: {
//                     scrollToBottom();
//                 }                    
//                
//
//            }
//            
//        }
//        EmotikonPicker {
//            id: emoticonsPicker
//            visible: true
//        }
//        }
//
//    }
//    actions: [
//        ActionItem {
//            title: qsTr("Send") + Retranslate.onLanguageChanged
//            imageSource: "asset:///icons/ic_send.png"
//            onTriggered: {
//                sendMessage();
//            }
//            ActionBar.placement: ActionBarPlacement.OnBar            
//        },
//        ActionItem {
//            title: qsTr("Send picture") + Retranslate.onLanguageChanged
//            imageSource: "asset:///icons/ic_doctype_picture.amd"
//            attachedObjects: [
//                FilePicker {
//                    id: fileImgPicker
//                    type: FileType.Picture
//                    title: qsTr("Select Picture") + Retranslate.onLanguageChanged
//                    mode: FilePickerMode.Picker
//                    directories: [ "/accounts/1000/shared/camera", "/accounts/1000/shared/pictures"]
//                    onFileSelected: {
//                        console.log("FileSelected signal received : " + selectedFiles);
//                        myVKChatController.sendImage(selectedFiles[0]);
//                    }
//                }
//            ]
//            onTriggered: {
//                fileImgPicker.open();
//            }            
//        },
////        ActionItem {
////            title: qsTr("Send music") + Retranslate.onLanguageChanged
////            imageSource: "asset:///icons/ic_doctype_music.amd"
////            onTriggered: {
////            
////            } 
////        },
////        ActionItem {
////            title: qsTr("Send file") + Retranslate.onLanguageChanged
////            imageSource: "asset:///icons/ic_doctype_generic.amd"
////            onTriggered: {
////            
////            } 
////        },
//        ActionItem {
//            title: qsTr("Send location") + Retranslate.onLanguageChanged
//            imageSource: "asset:///icons/ic_location.png"
//            onTriggered: {
//                placePicker.selectPlace();
//                console.log();
//                if (!isNaN(placePicker.selectedPlace.latitude))
//                {
//                    console.log("Sending location" + "lat:"+placePicker.selectedPlace.latitude + "long:"+placePicker.selectedPlace.longitude)
//                    myVKChatController.sendCurrentLocation(placePicker.selectedPlace.latitude, placePicker.selectedPlace.longitude);
//                }
//                else 
//                {
//                    console.log("Location unknown, nothing to send");
//                }
//            } 
//        }        
//    ]
//    attachedObjects: [
//        PlacePicker {
//            id: placePicker
//            
//            property variant selectedPlace
//            
//            function selectPlace()
//            {
//                selectedPlace = show()
//            }
//        }
//    ]
//    attachedObjects: [
//        // This handler is tracking the layout frame of the button.
//        LayoutUpdateHandler {
//            id: handler
//            onLayoutFrameChanged: {
//                // Individual layout frame values can be
//                // retrieved from the signal parameter                 
//                emoticonHeight = Math.floor(layoutFrame.height/3)
//            }
//            onCreationCompleted: {
//                emoticonHeight = Math.floor(layoutFrame.height/3)
//            }
//        }
//    ]
}
