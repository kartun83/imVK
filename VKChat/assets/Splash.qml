import bb.cascades 1.4
import bb.system 1.2
import bb.data 1.0
import smaatosdk 1.0
import bb.device 1.4

import "utilities.js" as Utilities

    Container {                  
        id: splashContainer
        background: back.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.XY
                imageSource: "asset:///splash/textura_bel_200.amd"
            }
        ]
        verticalAlignment: VerticalAlignment.Fill
        layout: DockLayout {

        }
        ImageView {
            accessibility.name: qsTr("Application logo")
            imageSource: "asset:///splash/splash.png"
//            imageSource: "asset:///splash/junk_large.png"
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            preferredWidth: ui.sdu(50)
            preferredHeight: ui.sdu(50)
            scalingMethod: ScalingMethod.AspectFit
        }
        Container {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            bottomPadding: ui.du(3.0)
//            ActivityIndicator {
//                running: false
//                accessibility.labelledBy: connectingLabel
//                verticalAlignment: VerticalAlignment.Center
//
//            }
            // Banner
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
            
            SSmaatoAdView {
            id: adView
            adSpaceId: "130071114"
            publisherId: "1100015701"
            preferredWidth: 320
            preferredHeight: 50
            viewSize: SSmaatoAdView.AdViewSizeNormal
            format: 1
            coppa: 0
            age: 30
//            gender: SSmaatoAPI.Male
//            tags: "motorsport,travel"
//            queryString: "london,manchester"
//            zip: "M17 1LB"
            autoRefreshPeriod: 20
            onAdUpdated: {
                if(success) {
                    // code for success update
                    console.log("Banner loaded");
                    } else {
                // code for failed update
                        console.log("Banner loading failed");
                    }
                }
            }
            /// Banner end
            Label {
                id: connectingLabel
                text: qsTr("Connecting")
                textStyle.fontStyle: FontStyle.Normal
                textStyle.fontWeight: FontWeight.W700
                textStyle.fontSize: FontSize.Large
                verticalAlignment: VerticalAlignment.Center
                textStyle.color: Color.create("#435D77")
                }
            }
        }
        Auth {
        
        }    
    }    
//}
