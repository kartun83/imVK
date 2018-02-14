import bb.cascades 1.4

Page {
    titleBar: TitleBar {
        // Localized text with the dynamic translation and locale updates support
        title: qsTr("About imVK") + Retranslate.onLanguageChanged
    }
    Container {
        layout: StackLayout {
        
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill            
            //            background: Color.Yellow            
            topPadding: ui.sdu(2.0)
            bottomPadding: ui.sdu(2.0)
            ImageView {
                imageSource: "asset:///splash/splash.png"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: ui.sdu(30)
                preferredHeight: ui.sdu(30)
            }
        }
        Container {
            leftPadding: ui.sdu(2.0)
            Label {
                text: qsTr("Author: %1").arg("Alexey Tveritinov") + Retranslate.onLocaleOrLanguageChanged
            }
            Label {
                text: qsTr("Contacts:") + Retranslate.onLocaleOrLanguageChanged;
            }
            Container {
                leftPadding: ui.sdu(2.0)                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight                        
                    }
                    Label {
                        text: "BBM:"
                    }
                    Label {
                        text: "2BE5F114"
                        content.flags: TextContentFlag.ActiveText
                        textFormat: TextFormat.Auto                        
                        textStyle{
                            fontWeight: FontWeight.Bold
                            //                            color: ui.palette.textOnPrimary
                        
                        }
                    
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight                        
                    }
                    Label {
                        text: "email:"
                    }
                    Label {
                        text: "kartun@yandex.ru"
                        textFormat: TextFormat.Auto
                        onTouch: {                         
                            emailInvocation.query.uri = "mailto:kartun@yandex.ru?subject=imVK&body=Hello there! "+
                                                        "I'm using imVK and I'd like to share my experience...";
                            emailInvocation.query.updateQuery();                                                       
                            emailInvocation.trigger("bb.action.SENDMAIL");
                        }
                        textStyle.fontWeight: FontWeight.Bold
                        textStyle.color: Color.Blue
                        content.flags: TextContentFlag.ActiveText
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight                        
                    }

                    Label {
                        text: qsTr("Donations:")  + Retranslate.onLocaleOrLanguageChanged
                        multiline: true
                    }
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
                    
                        Label {
                            text: "https://www.paypal.me/Kartun"
                            textFormat: TextFormat.Auto                            
                            textStyle.fontWeight: FontWeight.Bold
                            textStyle.color: Color.Blue              
                            content.flags: TextContentFlag.ActiveText          
                        }
                        Label {
                            text: "https://yasobe.ru/na/imvk"
                            textFormat: TextFormat.Auto                            
                            textStyle.fontWeight: FontWeight.Bold
                            textStyle.color: Color.Blue       
                            content.flags: TextContentFlag.ActiveText                 
                        }
                    }
                } // Donations
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight                        
                    }
                    Label {
                        text: qsTr("Usergroup:") + Retranslate.onLocaleOrLanguageChanged
                    }
                    Label {
                        text: "https://vk.com/imvk10"
                        textFormat: TextFormat.Auto
//                        onTouch: {
//                            donationInvocation.query.uri = "https://www.paypal.me/Kartun"
//                            donationInvocation.trigger("bb.action.OPEN");
//                        }
                        textStyle.fontWeight: FontWeight.Bold
                        textStyle.color: Color.Blue
//                        textStyle.color: Color.Blue              
                        content.flags: TextContentFlag.ActiveText          
                    }
                }
            
            }
        }
    
    }
    attachedObjects: [
        Invocation {
            id: emailInvocation
            query.mimeType: "text/plain"
            query.invokeTargetId: "sys.pim.uib.email.hybridcomposer"
            query.invokeActionId: "bb.action.SENDEMAIL"            
            onArmed: {
                emailInvocation.trigger(emailInvocation.query.invokeActionId);
            }
        }
//        Invocation {
//            id: donationInvocation
//            query: InvokeQuery
//            {
//                mimeType: "text/html"
////                uri: "https://www.paypal.me/Kartun"
//                invokeActionId: "bb.action.OPEN"
//            }
//        }
    ]
}
