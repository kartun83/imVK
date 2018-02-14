import bb.cascades 1.4
import bb.multimedia 1.4
import "utilities.js" as Utilities

Container {
    id: musicPlayerContainer
    property bool playing;
    onCreationCompleted: {
        playing = false;
        myVKChatController.mediaPlayer.mediaStateChanged.connect(changePlayingState)     
    }
    function changePlayingState(NewState)
    {
        if ( NewState == MediaState.Paused)
        {
            stateIcon.imageSource = "asset:///icons/ic_play.amd"
            playing = false;
        }
        else 
        {
            playing = true;
            console.log(myVKChatController.mediaPlayer.metaData);
            stateIcon.imageSource = "asset:///icons/ic_pause.amd"
        }
    }
        
    background: ui.palette.plainBase
    preferredHeight: ui.sdu(9.0)
    maxHeight: preferredHeight
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    ImageView {
        id: stateIcon
        accessibility.name: qsTr("Play/pause icon");
        imageSource: "asset:///icons/ic_play.amd"
        filterColor: ui.palette.textOnPlain
        minWidth: ui.sdu(9.0)
        minHeight: minWidth
        onTouch: {
            if (event.isUp())
            {
                if (playing)
                {
                    myVKChatController.mediaPlayer.pause();
                    playing = false;                    
                }       
                else 
                {
                    myVKChatController.mediaPlayer.play();
                    playing = true;
                }         
            }
        }
    }
    Container {
        rightPadding: ui.sdu(2.0)
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        Container {
            id: titleAndDurCont
            preferredHeight: ui.sdu(5.0)
            horizontalAlignment: HorizontalAlignment.Fill
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                id: songArtist
//                text: "Jaskaz - пули вавилона, двухкуплетная версия цуклшпаищшвипу"
                text: myVKChatController.songTitle
                horizontalAlignment: HorizontalAlignment.Fill
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
            }
            Label {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: -1
                }
                id: songProgress
                text: Utilities.fmtDuration( myVKChatController.mediaPlayer.duration / 1000 )
//                text: "15:23"
                horizontalAlignment: HorizontalAlignment.Right
            }
        }
        
        Container {
        id: progrAndBitrateCont    
        topPadding: ui.sdu(1.0)
        
        horizontalAlignment: HorizontalAlignment.Fill
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        ProgressIndicator {
            accessibility.name: qsTr("Playback progress") + Retranslate.onLocaleOrLanguageChanged
            verticalAlignment: VerticalAlignment.Center
            id: playProgress
            value: myVKChatController.mediaPlayer.position
//              value: 100
//            toValue: Utilities.fmtDuration( myVKChatController.mediaPlayer.duration )
            toValue: myVKChatController.mediaPlayer.duration
            rightMargin: ui.sdu(2.0)
            leftMargin: ui.sdu(2.0)
            
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            
        }
        Label {
            layoutProperties: StackLayoutProperties {
                spaceQuota: -1
            }
            id: bitrateLabel
            text: myVKChatController.mediaPlayer.metaData.BitRate
//            text: "192"
            horizontalAlignment: HorizontalAlignment.Right            
        }
       }
        //        Label {
//            id: songTitle
//            text: "Some song"
//        }
        
    }
}
