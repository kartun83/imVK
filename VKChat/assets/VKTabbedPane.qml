import bb.cascades 1.4
import bb.system 1.2
import bb.data 1.0

import "utilities.js" as Utilities

TabbedPane {     
    Menu.definition: MenuDefinition {
        id: mainMenuDefinition
        // Add a Help action
        helpAction: HelpActionItem {
            title: qsTr("About") + Retranslate.onLanguageChanged
            imageSource: "asset:///icons/ic_info.png"
            onTriggered: {                
                    console.log("About: ActionItem : onTriggered");
                    // For Settings, we will use NavigationPane
                    var aboutObj = aboutDefinition.createObject();
                    Application.menuEnabled = false;
                    currentNavigationPane.push(aboutObj);
            }
        }
        
        // Add a Settings action
        settingsAction: SettingsActionItem {
            title: qsTr("Settings") + Retranslate.onLanguageChanged
            imageSource: "asset:///icons/ic_settings.png"
            onTriggered: {
                console.log("Settings: ActionItem : onTriggered");
                // For Settings, we will use NavigationPane
                var settObj = settingsDefinition.createObject();
                Application.menuEnabled = false;
                currentNavigationPane.push(settObj);
            }
        }            
    }    
    /* Note: 
     * To push a NavigationPane Page from the ApplicationMenu, we will need  
     * to keep track which NavigationPane to push it to (that is, which is the active NavigationPane)
     * Tab cannot be added to NavigationPane (but the other way around is possible)
     */ 
    property NavigationPane currentNavigationPane: contactsTab.navHandle
    
    // Update the NavigationPane handle when the Tab is changed
    onActiveTabChanged: {
        console.log("TabbedPane : onActiveTabChanged : activeTab.description = " + activeTab.description);
        currentNavigationPane = activeTab.navHandle
    }                                            
    tabs: [                        
        Tab {
            id: contactsTab
            property alias navHandle : contactsDelegate.object
            title : qsTr("Contacts") + Retranslate.onLanguageChanged
            description : qsTr("%1 contacts").arg(myVKChatController.countactCount)//qsTr("List of contacts") + Retranslate.onLanguageChanged
//            description : qsTr("%1 contacts").arg(7)//qsTr("List of contacts") + Retranslate.onLanguageChanged
            imageSource : "asset:///icons/ic_contact.png"
            ActionBar.placement: ActionBarPlacement.OnBar            
            
            // Default delegateActivationPolicy explicitly stated for clarity only, the delegates object
            // (the ContinentsTab) will be created/destroyed when the tab is selected/unselected.
            delegateActivationPolicy: TabDelegateActivationPolicy.Default
            
            delegate: Delegate {
                id: contactsDelegate
                source: "Contacts.qml"
//                onCreationCompleted: {
//                    contactsTab.navHandle = contactsNaviPane  
//                }
            }            
        },
        Tab {
            id: commTab
            property alias navHandle : commDelegate.object 
            title : qsTr("Communities") + Retranslate.onLanguageChanged
            description : qsTr("List of communities") + Retranslate.onLanguageChanged
            imageSource : "asset:///icons/ic_groups.png"
            ActionBar.placement: ActionBarPlacement.OnBar            
            
            // Default delegateActivationPolicy explicitly stated for clarity only, the delegates object
            // (the ContinentsTab) will be created/destroyed when the tab is selected/unselected.
            delegateActivationPolicy: TabDelegateActivationPolicy.Default
            
            delegate: Delegate {
                id: commDelegate
                source: "Communities.qml"
//                onCreationCompleted: {
//                     commTab.navHandle = commNaviPane;
//                }
            }
        },
        Tab {
            id: diaTab
            property alias navHandle : diaDelegate.object
            title: qsTr("Dialogs") + Retranslate.onLanguageChanged
            description: qsTr("Dialogs") + Retranslate.onLanguageChanged
            imageSource: "asset:///icons/ic_send.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            delegateActivationPolicy: TabDelegateActivationPolicy.Default
            delegate: Delegate {
                id: diaDelegate
                source: "DialogsPage.qml"
//                onCreationCompleted: {
//                    diaTab.navHandle = diaNaviPane
//                }
            }
        },
        Tab {
            id: musicTab
            property alias navHandle : musicDelegate.object
            title: qsTr("Music") + Retranslate.onLanguageChanged
            description: qsTr("Music") + Retranslate.onLanguageChanged
            imageSource: "asset:///icons/ic_play.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            delegateActivationPolicy: TabDelegateActivationPolicy.Default
            delegate: Delegate {
                id: musicDelegate
                source: "MusicPage.qml"
//                onCreationCompleted: {
//                    musicTab.navHandle = musicNaviPane;
//                }
            }
            
            //                            MusicPage {
            //                                
            //                            }
        }
    ]
    showTabsOnActionBar: false


    onCreationCompleted: {
        splashSheet.open();
        myVKChatController.authTokenReceived.connect(showTabbedUI);
        Application.setCover(multiCover);
//        Application.swipeDown.connect(onSwipeDown);                
//        Application.setMenu(mainMenuDefinition);
//        console.log("Menu %1").arg(Application.menu);
        
        console.log("Menu enabled:" + Application.menuEnabled);            
        //        splashSheet.open();
        //        tabSheet.open();        
    }
    
    function showTabbedUI()
    {
        console.log("Displaying tab UI");
//        tabSheet.open();
        splashSheet.close();
//        Application.setMenuEnabled(true);
//        var abc = Application.menu;
        console.log("Menu enabled:" + Application.menuEnabled);
    }     
    
    attachedObjects: [
        
        Sheet {
            peekEnabled: false
//            id: tabSheet
            id: splashSheet
            content: 
                Page { Splash { } }                 
        },
        ComponentDefinition {
            id: aboutDefinition
            source: "AboutBox.qml"
        },
        ComponentDefinition {
            id: settingsDefinition
            source: "SettingsPage.qml"
        },
        MultiCover {
            id: multiCover
            
            SceneCover {
                id: bigCover
                // Use this cover when a large cover is required
                MultiCover.level: CoverDetailLevel.Default// High
                content: ActiveCover {     
                    // Here would be only incoming messages
                    property int contactID : myVKChatController.latestMessage.from_id //ListItemData.out == true ? myVKChatController.latestMessage.user_id : myVKChatController.latestMessage.from_id   
                    firstName: myVKChatController.getContactFullName( contactID )                       
                    body: myVKChatController.latestMessage.body
                    when: Utilities.fmtTime( myVKChatController.latestMessage.date )
                    avatar: myVKChatController.getContact(contactID).photo_100
                    status: myVKChatController.getContact(contactID).status
                    online: myVKChatController.getContact(contactID).online
                } 
                function update() {
                    // Update the large cover dynamically
                }
            } // sceneCover HIGH
            
            SceneCover {
                id: smallCover
                // Use this cover when a small cover is required
                MultiCover.level: CoverDetailLevel.Medium
                content: ActiveCoverSmall {
                    sender: myVKChatController.getContactFullName( myVKChatController.latestMessage.recepientID )
                    body:   myVKChatController.latestMessage.body
                    when:   Utilities.fmtTime( myVKChatController.latestMessage.date )
                }
                function update() {
                    // Update the small cover dynamically
                }
            } // sceneCover MEDIUM
            
            function update() {
                bigCover.update()
                smallCover.update()
            }
        } // MultiCover end
    ]                        
//}
}