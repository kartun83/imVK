import bb.cascades 1.4
import VKChat 1.0

NavigationPane {
    id: navigationPane
    
    Chat {
        
    }
//    Page {
//        Container {
//            layout: StackLayout {
//            
//            }
//            Container {
//                layout: StackLayout {
//                    orientation: LayoutOrientation.LeftToRight
//                }
//                Label {
//                    // Localized text with the dynamic translation and locale updates support
//                    text: qsTr("Hello Hub Integration") + Retranslate.onLocaleOrLanguageChanged
//                    textStyle.base: SystemDefaults.TextStyles.BigText
//                }
//            }
//        }
//    }
    
    onPopTransitionEnded: {
        _app.closeCard();
    }
}
