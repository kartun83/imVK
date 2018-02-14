/*
 * Copyright (c) 2011-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.4
import bb.system 1.2
import bb.data 1.0

import "utilities.js" as Utilities

//import kartun.vk_lib 1.0

NavigationPane {
//    Menu.definition: MenuDefinition {
//        // Add a Help action
//        helpAction: HelpActionItem {
//            title: qsTr("About")
//            imageSource: "asset:///icons/ic_info.png"
//        }
//        
//        // Add a Settings action
//        settingsAction: SettingsActionItem {
//            title: qsTr("Settings")
//            imageSource: "asset:///icons/ic_settings.png"
//        }            
//    }
    function parseUrl(querystring) {
        // remove any preceding url and split
        querystring = querystring.toString().substring(querystring.toString().indexOf('#')+1).split('&');
        var params = {}, pair, d = decodeURIComponent;
        // march and parse
        for (var i = querystring.length - 1; i >= 0; i--) {
            pair = querystring[i].split('=');
            params[d(pair[0])] = d(pair[1]);
        }
        
        return params;
    } //--  fn  deparam
    
    id: navigationPane
    //backButtonsVisible: false
    attachedObjects: [
        ComponentDefinition {
            id: musicDefinition
            source: "MusicPage.qml"
        },
        ComponentDefinition {
            id: chatDefinition
            source: "Chat.qml"
        },
        ComponentDefinition {
            id: contactsDefinition
            source: "Contacts.qml"
        },
        ComponentDefinition {
            id: contactInfoDefinition
            source: "contact_info.qml"
        },
        ComponentDefinition {
            id: oAuthDefinition
            source: "auth.qml"
        }
    ]
    Page {           
        onCreationCompleted: {
            Application.setCover(multiCover)
        } 
        
        attachedObjects: [
            //ComponentDefinition {
            //    id: authComponent
                Sheet {
                    id: authSheet
                    content: 
                    Page {
                        id: authPage
                        Container {                    
                            WebView {
                                url: "https://oauth.vk.com/authorize?client_id=4923095&redirect_uri=https://oauth.vk.com/blank.html&scope=friends,photos,audio,messages&display=mobile&response_type=token"
                            onUrlChanged: {
                                console.log(url);
                                var i = url.toString().indexOf("access_token=");
                                if (i>0)
                                {
                                    var params = parseUrl(url);
                                    console.log("User ID:"+params["user_id"]);
                                    console.log("Token:"+params["access_token"]);
                                    console.log("Expires in:"+params["expires_in"]);
                                    myVKChatController.setAuthData(params["access_token"], params["user_id"], params["expires_in"]);
                                    authSheet.close();
//                                    diveInButton.enabled = true;
//                                      var newPage = contactsDefinition.createObject();
//                                      navigationPane.push(newPage);
                                }
                                else 
                                {
                                    console.log("Token not found");
                                    // Here we should popup oAuth window
                                    var newPage = oAuthDefinition.createObject();
                                    navigationPane.push(newPage);
                                }
                            }
                            settings.zoomToFitEnabled: true
                            settings.credentialAutoFillEnabled: false
                            settings.formAutoFillEnabled: false

                        }
                        
                        }
                    }
                },
                MultiCover {
                    id: multiCover
                    
                    SceneCover {
                        id: bigCover
                        // Use this cover when a large cover is required
                        MultiCover.level: CoverDetailLevel.Default// High
                        content: ActiveCover {     
                            firstName: myVKChatController.getContactFullName( myVKChatController.latestMessage.from_id )                       
                            body: myVKChatController.latestMessage.body
                            when: Utilities.fmtTime( myVKChatController.latestMessage.date )
                            avatar: myVKChatController.getContact(myVKChatController.latestMessage.from_id).photo_100
                            status: myVKChatController.getContact(myVKChatController.latestMessage.from_id).status
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
                            sender: myVKChatController.getContactFullName( myVKChatController.latestMessage.from_id )
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
                }
            //}
        ]
        
//        Splash {
//            
//        }

    }
    onPopTransitionEnded: 
    {         
        console.log("Destroying:" + page.objectName);
        switch (page.objectName)
        {
            case "chatPage" : myVKChatController.unsetCurrentContact();
        }        
        page.destroy(); 
    }
}
