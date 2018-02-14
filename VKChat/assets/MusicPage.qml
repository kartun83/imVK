import bb.cascades 1.4
import smaatosdk 1.0

import "utilities.js" as Utilities

NavigationPane {
    id: musicNaviPane  
Page {
    id: musicPage
    titleBar: TitlebarSeach {
        id: musicTitleBar
    }
    property bool ascending : true
    property int mode : 1 // Modes : 1 - own music, 2 - search
    Container {
        SSmaatoAdView {
            id: adView
            adSpaceId: "130071187"
            publisherId: "1100015701"
            preferredWidth: 728
            preferredHeight: 90
            viewSize: SSmaatoAdView.AdViewSizeNormal
            format: 1
            coppa: 0
//            age: 30
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
//        background: ui.palette.plainBase
//        Label {
//            text: "Here would be music"
//        }
        ListView {
            id: musicList
            accessibility.name: qsTr("MusicList") + Retranslate.onLocaleOrLanguageChanged;
            accessibility.description: qsTr("List of available music") + Retranslate.onLocaleOrLanguageChanged;
            dataModel: myVKChatController.musicModel
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        margin.leftOffset: ui.sdu(2.0)                        
                        title: ListItemData.artist
                        description: ListItemData.title
                        status: Utilities.fmtDuration(ListItemData.duration)                        
                        //imageSource: "asset:///icons/ic_play.amd"   
                        onTouch: {
                            if (event.isUp())
                            {
                                Qt.myVKChatController.play(ListItemData.id);
//                                Qt.myVKChatController.play(ListItemData.url, ListItemData.artist, ListItemData.title);
                            }
                        }  
                        contextActions: [
                            
                            ActionSet {
                                ActionItem {
                                    title: qsTr("Add to my music") + Retranslate.onLanguageChanged
                                    imageSource: "asset:///icons/ic_add_bookmarks.png"
                                    onTriggered: {
                                        Qt.myVKChatController.addSongToMyList(ListItemData.id, ListItemData.owner_id);        
                                    }
//                                    enabled: musicPage.mode == 1 ? false : true                                    
                                }
                                ActionItem {
                                    title: qsTr("Find similar") + Retranslate.onLanguageChanged
                                    imageSource: "asset:///icons/ic_popular.png"
                                    onTriggered: {
                                        Qt.myVKChatController.getRecommendedMusic(ListItemData.id, ListItemData.owner_id);        
                                    }
                                    //                                    enabled: musicPage.mode == 1 ? false : true                                    
                                }
                                DeleteActionItem {
                                    title: qsTr("Delete from my music") + Retranslate.onLanguageChanged
//                                    imageSource: "asset:///icons/ic_"
                                    onTriggered: {
                                        Qt.myVKChatController.deleteSongFromMyList(ListItemData.id, ListItemData.owner_id);
                                    }
                                }                                                            
                            }
                        ]                
//                        image: ImageView {
//                            filterColor: ui.palette.textOnPlain
                            
//                            minWidth: ui.px(64)
//                            minHeight: minWidth
//                        }                    
                    }    
                }
                
            ]
            snapMode: SnapMode.LeadingEdge
            flickMode: FlickMode.Default
            onCreationCompleted: {
                Qt.myVKChatController = myVKChatController;
            }
        }    // Listview
        PlayerControl {
            
        }
    }
    actions: [
        ActionItem {
            id: sortAction
            title: qsTr("New to old") + Retranslate.onLocaleOrLanguageChanged
            imageSource: "asset:///icons/ic_to_bottom.png"
            onTriggered: {
                musicPage.ascending = !musicPage.ascending;
                musicList.dataModel.setSortedAscending(musicPage.ascending);
                sortAction.imageSource = musicPage.ascending == false ? "asset:///icons/ic_to_bottom.png" : "asset:///icons/ic_to_top.png"
                sortAction.title = musicPage.ascending == false ? qsTr("Old to new") : qsTr("New to old")
            }    
              
        },
        ActionItem {
            title: qsTr("Search") + Retranslate.onLocaleOrLanguageChanged
            onTriggered: musicTitleBar.startSearch();
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///icons/ic_search.amd"
        },
        ActionItem {
            title: qsTr("Recommended music") + Retranslate.onLanguageChanged
            onTriggered: {
                myVKChatController.getRecommendedMusic();
            }
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///icons/ic_popular.png"
        },
        ActionItem {
            title: qsTr("Popular music") + Retranslate.onLanguageChanged
            onTriggered: {
                myVKChatController.getPopularMusic();
            }
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///icons/ic_popular.png"
        },
        ActionItem {
            title: qsTr("Equalizer") + Retranslate.onLocaleOrLanguageChanged
            onTriggered: {
                var eqPage = eqDefinition.createObject();                
                musicNaviPane.push(eqPage);
            }
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///icons/ic_eq.png"
        },
        ActionItem {
            title: qsTr("My music") + Retranslate.onLocaleOrLanguageChanged
            onTriggered: myVKChatController.updateMusicModel();
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///icons/ic_reload.png"                    
        }
    ] 
}
    onPopTransitionEnded: 
    {         
        console.log("Destroying:" + page.objectName);
//        switch (page.objectName)
//        {
//            case "chatPage" : myVKChatController.unsetCurrentContact();
//        }
        Application.menuEnabled = true;        
        page.destroy(); 
    }
    attachedObjects: [
        ComponentDefinition {
            id: eqDefinition
            source: "Equalizer.qml"
        }
    ]
}