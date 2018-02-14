import bb.cascades 1.4
import smaatosdk 1.0
import bb.device 1.4

//Page {
    Container {
        accessibility.name: qsTr("vkChat Status bar") + Retranslate.onLanguageChanged
        // Properties
        property int titlebarHeight: 0
        property string description
        property string imageSource
        property string firstName
        property string lastName
        property alias avatarImageViewData: avatarView.imageSource
        property bool online
        property alias showOnline : onlineIndicator.visible
        property bool showBanner : true
        /////////////
        horizontalAlignment: HorizontalAlignment.Fill
        attachedObjects: [
            LayoutUpdateHandler {
                id: titleBarLayout
                onLayoutFrameChanged: {
                    // Tracking the titlebar height to guide size of avatar
                    titlebarHeight = layoutFrame.height
                }
            }
        ]
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        background: ui.palette.background
        Container {
            layout: DockLayout {
                
            }
        
            ImageView {
                id: avatarView
                accessibility.name: qsTr("Contact avatar")
                objectName: "avatarView"
                preferredHeight: titlebarHeight 
                preferredWidth: titlebarHeight 
                minWidth: preferredWidth
                scalingMethod: ScalingMethod.AspectFill    
//                imageSource: "asset:///img/icon_110.png"        
            }
            
            ImageView {
                id: onlineIndicator
                accessibility.name: qsTr("Online indicator")
                imageSource: online ? "asset:///img/online.png" : "asset:///img/offline.png" 
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Bottom
                preferredHeight: ui.sdu(2.0)
                preferredWidth: preferredHeight
                maxHeight: preferredHeight
                maxWidth: preferredHeight
                margin.rightOffset: ui.sdu(0.2)
                margin.bottomOffset: ui.sdu(0.2)
//                visible: true
            }
            
        }
        Container // holds name & description
        {
            id: topContainer
            objectName: "topContainer"
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center            
            implicitLayoutAnimationsEnabled: false
            visible: true
            //background: wantsHighlight ? _UIStyleManager.highlightBackgroundColor : undefined            
            
            onCreationCompleted: {
                //_CompatibilityManager.setFocusable(nameAndDescription, true, false);
            }
            
//            layoutProperties: StackLayoutProperties {
//                spaceQuota: 2
//            }
            Container {
                id: nameAndDescription
                objectName: "nameAndDescription"

            leftPadding:    ui.sdu(2.0)
            
            Container {
                id: nameContainer    
                //topPadding:     ui.sdu(2.0)
//                layoutProperties: StackLayoutProperties {
//                    spaceQuota: -1
//                }
            Label {
                    objectName: "name"
                    verticalAlignment: VerticalAlignment.Fill
                    content.flags: TextContentFlag.Emoticons | TextContentFlag.ActiveTextOff
                    leftMargin: 0
                    text: qsTr("%1 %2").arg(firstName).arg(lastName)
                    textFormat: TextFormat.Plain
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    textStyle.color: ui.palette.textOnPlain
                    //textStyle.color: wantsHighlight ? _UIStyleManager.highlightTextColor : _UIStyleManager.titleBarPrimaryColor
                    textStyle.fontSize: FontSize.Medium
                    
//                    layoutProperties: StackLayoutProperties {
//                        spaceQuota: -1
//                    }
                }    // Label for name
            }
            
            Container {
                id: descriptionContainer                            
                Label {
                    objectName: "description"
                    verticalAlignment: VerticalAlignment.Fill
                    content.flags: TextContentFlag.Emoticons | TextContentFlag.ActiveTextOff                    
                    text: description
                    textFormat: TextFormat.Auto
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        fontStyle: FontStyle.Italic
                        fontSize: FontSize.Small
                        color: ui.palette.textOnPlain
                    }
                    
                    //textStyle.color: wantsHighlight ? _UIStyleManager.highlightTextColor : _UIStyleManager.titleBarSecondaryColor
                    
                    
                    // no horizontal fill or spaceQuota on purpose
                    // we want the label to be left aligned even with right-to-left text for consistency with the name label
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: -1
                    }
                    // Label for description
                }
            }       
            
            }
            
        }
        // Banner
//        Container {
//            layout: DockLayout {
//            
//            }
//            visible: showBanner
//            horizontalAlignment: HorizontalAlignment.Right        
//            SSmaatoAdView {
//                horizontalAlignment: HorizontalAlignment.Right
//                id: adView
//                adSpaceId: "130071174"
//                publisherId: "1100015701"
//                //                    adSpaceId: "130071174"
//                //                    publisherId: "1100015701"
//                
//                preferredWidth: 300
//                preferredHeight: titlebarHeight
//                viewSize: SSmaatoAdView.AdViewSizeMedium
//                format: 1
//                coppa: 0
//                //                    age: 30
//                //            gender: SSmaatoAPI.Male
//                //            tags: "motorsport,travel"
//                //            queryString: "london,manchester"
//                //            zip: "M17 1LB"
//                autoRefreshPeriod: showBanner == true ? 20 : 0
//                onAdUpdated: {
//                    if(success) {
//                        // code for success update
//                        console.log("Banner loaded");
//                    } else {
//                        // code for failed update
//                        console.log("Banner loading failed");
//                    }
//                }
//            }
//            /// Banner end
//        }
//        // End of banner
    Divider {
        accessibility.name: qsTr("divider") + Retranslate.onLanguageChanged
        topMargin: 0; bottomMargin: 0
    }

}
//}
