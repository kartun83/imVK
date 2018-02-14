import bb.cascades 1.4
import VKChat 1.0
//import smaatosdk 1.0
import bb.device 1.4

//Page {
//    titleBar: 
        TitleBar {
         function performSearch(searchTerm)
         {
             // Hide buttons
             cancelSearch();
             myVKChatController.searchMusic(searchTerm);
         }
         
         function cancelSearch()
         {
             cancelButton.visible = false;
             searchTermEdit.visible = cancelButton.visible;
             searchIcon.visible = !searchTermEdit.visible;
             headerLabel.visible = searchIcon.visible;            
         }
         
         // To be called from outside page
         function startSearch()
         {
             cancelButton.visible = true;
             searchTermEdit.visible = cancelButton.visible;
             searchIcon.visible = !searchTermEdit.visible;
             headerLabel.visible = searchIcon.visible;              
         }
        id: musicTitleBar
        title : qsTr("imVK music")
        scrollBehavior: TitleBarScrollBehavior.Sticky
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
        Container {            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
                leftPadding: ui.sdu(2.0)
                Label {
                    id: headerLabel
                    text: musicTitleBar.title 
                    verticalAlignment: VerticalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    textStyle.fontSize: FontSize.Large
                    textStyle.fontStyle: FontStyle.Default
                    textStyle.fontWeight: FontWeight.W500
                }
                 TextField {
                     id: searchTermEdit
                     hintText: qsTr("Search music")
                     layoutProperties: StackLayoutProperties {
                         spaceQuota: -1
                     }
                    textStyle.textAlign: TextAlign.Default
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Center
                    visible: false
                    
                    input.onSubmitted: {
                        musicTitleBar.performSearch(searchTermEdit.text);
                    }
                    input.submitKey: SubmitKey.Search

                }
                 Button {
                    id: cancelButton
                    text: qsTr("Cancel")
                    appearance: ControlAppearance.Plain
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    maxWidth: ui.sdu(17.0)
                    visible: false
                    layoutProperties: StackLayoutProperties {
                         spaceQuota: 2
                     }
                     onTouch: {
                         if (event.isUp())
                         {
                             musicTitleBar.cancelSearch();
                         }
                     }
                     
                 }
                 
//                 Container {
//                     id: bannerContainer
//                     layout: DockLayout {
//                     
//                     }
//                     visible: searchIcon.visible
//                     horizontalAlignment: HorizontalAlignment.Right        
//                     SSmaatoAdView {
//                         horizontalAlignment: HorizontalAlignment.Right
//                         id: adView
//                         adSpaceId: "130071187"
//                         publisherId: "1100015701"
//                         //                    adSpaceId: "130071174"
//                         //                    publisherId: "1100015701"
//                         
//                         preferredWidth: 300
//                         preferredHeight: 250
//                         viewSize: SSmaatoAdView.AdViewSizeMedium
//                         format: 1
//                         coppa: 0
//                         //                    age: 30
//                         //            gender: SSmaatoAPI.Male
//                         //            tags: "motorsport,travel"
//                         //            queryString: "london,manchester"
//                         //            zip: "M17 1LB"
//                         autoRefreshPeriod: 20
//                         onAdUpdated: {
//                             if(success) {
//                                 // code for success update
//                                 console.log("Banner loaded");
//                             } else {
//                                 // code for failed update
//                                 console.log("Banner loading failed");
//                             }
//                         }
//                     }
//                     /// Banner end
//                 }
//                 // End of banner
                 
                 ImageView {
                     id: searchIcon
                     accessibility.name: qsTr("Start search")
                     imageSource: "asset:///icons/ic_search.amd"
                     filterColor: ui.palette.textOnPlain
                     verticalAlignment: VerticalAlignment.Center
                     horizontalAlignment: HorizontalAlignment.Right
                     onTouch: {
                         if (event.isUp())
                         {
                             musicTitleBar.startSearch();                             
                         }
                     }
                 }
            } // End of titlebar content
        }
    } // End of title bar
//    Container {
//        
//    }
//}
