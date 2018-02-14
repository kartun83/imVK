import bb.cascades 1.4

//Page {
ListItemComponent {    
    
    type: "item"
    Container {
        id: top_level
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom        
        }        
    
    Container {
            id: content_level

            function fmtTime(inSeconds) {
                var hours = Math.floor(inSeconds / 3600);
                var minutes = Math.floor((inSeconds - (hours * 3600)) / 60);
                var seconds = inSeconds - (hours * 3600) - (minutes * 60);
                var time = "";

                if (hours != 0) {
                    time = hours + ":";
                }
                if (minutes != 0 || time !== "") {
                    minutes = (minutes < 10 && time !== "") ? "0" + minutes : String(minutes);
                    time += minutes + ":";
                }
                if (time === "") {
                    time = seconds + "s";
                } else {
                    time += (seconds < 10) ? "0" + seconds : String(seconds);
                }
                return time;
            }

            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight

            }

            leftPadding: ui.sdu(2.0)
            rightPadding: ui.sdu(2.0)
            ImageView {
                id: statusIcon
                imageSource: {
                    content_level.determineIcon(ListItemData.status);
                }
                //imageSource: "asset:///icons/ic_play.png"
                accessibility.name: "state_icon"
                accessibility.description: "State icon"
                preferredWidth: 96.0
                preferredHeight: 96.0
                minWidth: 96.0
                filterColor: ui.palette.textOnPlain
                visible: false
                //filterColor: Color.Transparent

            }
            Container {
                layout: StackLayout {

                }
                Label {
                    id: labelName

                    text: ListItemData.name
                    textStyle.fontWeight: FontWeight.Bold
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        id: labelTotalSize
                        text: qsTr("%1: %2").arg("Size").arg(content_level.formatBytes(ListItemData.sizeWhenDone, 2)) + Retranslate.onLocaleOrLanguageChanged
                        onTextChanged: {
                            ListItemData.percentDone >= 1 ? labelTotalSize.visible = false : labelTotalSize.visible = true;
                        }
                    }
                    Label {
                        id: labelDownloaded
                        text: qsTr("%1: %2").arg("DL").arg(content_level.formatBytes(ListItemData.downloadedEver, 2)) + Retranslate.onLocaleOrLanguageChanged
                        onTextChanged: {
                            ListItemData.percentDone >= 1 ? labelDownloaded.visible = false : labelDownloaded.visible = true;
                        }
                    }
                    Label {
                        id: labelDownloadSpeed
                        text: qsTr("%1: %2/s").arg("DL speed").arg(content_level.formatBytes(ListItemData.rateDownload, 2)) + Retranslate.onLocaleOrLanguageChanged
                        onTextChanged: {
                            if ( ( ListItemData.percentDone < 1 ) && (ListItemData.rateDownload  > 0) )  
                            { labelDownloadSpeed.visible = true } else { labelDownloadSpeed.visible = false; }                            
                        }
                    }
                    
                    Label {
                        id: labelUploaded
                        text: qsTr("%1: %2").arg("UL").arg(content_level.formatBytes(ListItemData.uploadedEver, 2)) + Retranslate.onLocaleOrLanguageChanged
                        onTextChanged: {
                            ListItemData.percentDone < 1 ? labelUploaded.visible = false : labelUploaded.visible = true;
                        }
                    }
                    
                    Label {
                        id: labelUploadRatio
                        text: qsTr("%1: %2").arg("Ratio").arg(ListItemData.uploadRatio) + Retranslate.onLocaleOrLanguageChanged
                        onTextChanged: {
                            ListItemData.percentDone < 1 ? labelUploadRatio.visible = false : labelUploadRatio.visible = true;
                        }
                    }
                    
                    Label {
                        id: labelUploadSpeed
                        text: qsTr("%1: %2/s").arg(qsTr("UL speed")).arg(content_level.formatBytes(ListItemData.rateUpload, 2)) + Retranslate.onLocaleOrLanguageChanged
                        onTextChanged: {
                            if ( ( ListItemData.percentDone >= 1 ) && (ListItemData.rateUpload  > 0) )  
                            { labelUploadSpeed.visible = true; } else { labelUploadSpeed.visible = false; }
                        }
                    }
                    
                    Label {
                        id: labelETA
                        text: qsTr("%1: %2").arg(qsTr("ETA")).arg(content_level.fmtTime(ListItemData.eta))
                        onTextChanged: {
                            ListItemData.eta == -1 ? labelETA.visible = false : labelETA.visible = true;
                        }
                    }
                    onCreationCompleted: {

                        if (ListItemData.percentDone != 1) {
                            labelUploaded.visible = false;
                            labelUploadRatio.visible = false;
                            labelUploadSpeed.visible = false;
                            labelDownloaded.visible = true;
                            labelDownloadingInfo.visible    = true;
                            ListItemData.rateDownload != 0 ? labelDownloadSpeed.visible = true : labelDownloadSpeed.visible = false ;
                            //labelTotalSize.visible  = true;
                        } else {
                            labelUploaded.visible = true;
                            labelUploadRatio.visible = true;
                            ListItemData.rateUpload != 0 ? labelUploadSpeed.visible = true : labelUploadSpeed.visible = false;
                            labelDownloaded.visible = false;
                            labelDownloadSpeed.visible = false;
                            labelDownloadingInfo.visible    = false;
                            
                            //labelTotalSize.visible  = false;
                        }
                    }
                }
                ProgressIndicator {
                    id: progress
                    accessibility.name: "percent_completed"
                    toValue: 100
                    value: ListItemData.percentDone * 100

                }
                Label {
                    id: labelSeedingInfo
                    text: qsTr("%1 %2 %3").arg("Seeding to").arg(ListItemData.peersGettingFromUs).arg("peers")
                    onCreationCompleted: {
                        if ((ListItemData.percentDone < 1)  || 
                        (ListItemData.peersGettingFromUs <= 0) ) { labelSeedingInfo.visible = false; } 
                        else { labelSeedingInfo.visible = true;}
                    }
                    onTextChanged: {
                        if ((ListItemData.percentDone < 1)  || 
                            (ListItemData.peersGettingFromUs <= 0) ) { labelSeedingInfo.visible = false; } else { labelSeedingInfo.visible = true; }
                    }
                }
                Label {
                    id: labelDownloadingInfo
                    text: qsTr("%1 %2/%3").arg("Downloading from").arg(ListItemData.peersSendingToUs).arg(ListItemData.peersConnected)
                    
                    onTextChanged: {
                        if ((ListItemData.percentDone < 1)  || 
                        (ListItemData.peersSendingToUs <= 0) ) { labelDownloadingInfo.visible = false;} else { labelDownloadingInfo.visible = true; }                        
                    }
                    onCreationCompleted: {
                        if ((ListItemData.percentDone < 1)  || 
                        (ListItemData.peersSendingToUs <= 0) ) { labelDownloadingInfo.visible = false;} else { labelDownloadingInfo.visible = true; }
                    }
                }
            }

        }
        Divider {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            accessibility.name: "divider"
            accessibility.description: "divider"
        }
        contextActions: [
            ActionSet {
                title: qsTr("Actions") + Retranslate.onLocaleOrLanguageChanged
                subtitle: ListItemData.name
                
                actions: [
                    ActionItem {
                        title: ListItemData.status < 2 ? qsTr("Start") : qsTr("Stop")
                        imageSource: ListItemData.status < 2 ? "asset:///icons/ic_play.amd" : "asset:///icons/ic_stop.amd"
                        onTriggered: {
                            Qt.myTransController.toggleTorrent(ListItemData.id);
                        }

                    },
                    ActionItem {
                        title: qsTr("Info") + Retranslate.onLocaleOrLanguageChanged                        
                        imageSource: "asset:///icons/ic_info.png"
                        onTriggered: {
                            Qt.myTransController.getTorrentInfo(ListItemData.id);
                            var newPage = Qt.torrentInfoDefinition.createObject();
                            Qt.navigationPane.push(newPage); 
                        }
                    },
                    DeleteActionItem {
                        title: qsTr("Delete") + Retranslate.onLocaleOrLanguageChanged                                           
                        imageSource: "asset:///icons/ic_delete.png"
                        
                        onTriggered: {
                            Qt.myDeleteDialog.torrentName = ListItemData.name;
                            Qt.myDeleteDialog.torrentId   = ListItemData.id;
                            Qt.myDeleteDialog.show();
                        }                        
                        defaultAction: true
                    }
                    
                ]
            } // end of ActionSet   
        ]
        //leftPadding: ui.sdu(7.0) // end of contextActions list
    }    
}
