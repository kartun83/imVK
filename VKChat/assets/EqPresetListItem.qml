import bb.cascades 1.4
//import mm.extension 1.0
//import mm.extension.internal 1.0
import bb.system 1.0

CustomListItem {
    id: eqPresetItem
    
    //Standard CustomListItem properties
    dividerVisible: true
    highlightAppearance: HighlightAppearance.Frame
    
    property alias name: label.text
    //Is this the currently active Equalizer Preset?
    property bool isActive: false
    navigation {
        defaultHighlightEnabled: false
    }
    
    horizontalAlignment: HorizontalAlignment.Fill
     
    Container {
        background: eqPresetItem.navigation.wantsHighlight ? ui.palette.primaryDark : Color.Transparent
        layout: DockLayout {
        }
        //Chose this to try and match the size
        //of the StandardListItem's used by Audio Routing List
        minHeight: ui.sdu(11)
        horizontalAlignment: HorizontalAlignment.Fill

        Container {
            
            leftPadding: ui.sdu(2)
            rightPadding: ui.sdu(2)
            bottomPadding: ui.sdu(1)
    
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment:  VerticalAlignment.Center
    
            Label {
                accessibility.name: qsTr("Equalizer preset name") + Retranslate.onLanguageChanged
                touchPropagationMode: TouchPropagationMode.Full
                id: label
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.base: SystemDefaults.TextStyles.TitleText
//                textStyle.color: isActive ? (eqPresetItem.navigation.wantsHighlight ? ui.palette.secondaryTextOnPrimaryDark 
//                : ui.palette.secondaryTextOnPrimaryDark) : ui.palette.textOnPrimary
                textStyle {
//                    color: isActive ? (eqPresetItem.navigation.wantsHighlight ? ui.palette.secondaryTextOnPrimaryDark 
//                                : ui.palette.secondaryTextOnPrimaryDark) : ui.palette.textOnPrimary
                    color: isActive ? ui.palette.primarySoft : ui.palette.textOnPlain
                }
//                textStyle.color: ui.palette. 
                textStyle.fontSize: FontSize.Large
                textStyle.fontWeight: FontWeight.W400
            }
        }
    }
}
