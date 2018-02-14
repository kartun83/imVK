import bb.cascades 1.4

Page {
    titleBar: TitleBar {
        // Localized text with the dynamic translation and locale updates support
        title: qsTr("imVK settings") + Retranslate.onLocaleOrLanguageChanged
    }
    ScrollView {
        Container {
            layout: StackLayout {
            
            }
            leftPadding: ui.sdu(1.0)
            rightPadding: leftPadding
            topPadding: leftPadding 
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                
                DropDown {
                    id: themeDropDown
                    title: qsTr("Application theme")
                    options: [
                        Option {
                            text: qsTr("Bright") + Retranslate.onLanguageChanged
                            value: VisualStyle.Bright
                        },                        
                        Option {                            
                            text: qsTr("Dark") + Retranslate.onLanguageChanged
                            value: VisualStyle.Dark
                        }                                        
                    ]
                    onCreationCompleted: {
                        themeDropDown.setSelectedIndex(Application.themeSupport.theme.colorTheme.style - 1)
                    }
                    onSelectedValueChanged: {
                        if (!selectedValue.isNull)
                        {
                            Application.themeSupport.setVisualStyle(selectedValue);
                            myVKChatController.savePreferredTheme(selectedValue);
                        }                        
                    }
                }

            }
        
        }    
    }
    
}
