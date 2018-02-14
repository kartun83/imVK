import bb.cascades 1.4

Container {
//    background: Color.Red
  layout: StackLayout {
      orientation: LayoutOrientation.LeftToRight
  }  
//  horizontalAlignment: HorizontalAlignment.Fill
  Label {
      text: unescape(ListItemData.body)
//      text: "Map link to Moscow, Russia"
      id: bodyLabel
      multiline: true
      textStyle.color: ui.palette.textOnPlain
      content.flags: TextContentFlag.ActiveText | TextContentFlag.Emoticons
      textFormat: TextFormat.Auto
      layoutProperties: StackLayoutProperties {
          spaceQuota: 1
      }        
      onCreationCompleted: {
          if (bodyLabel.text == "")
          {
              bodyLabel.visible = false;
          }
          horizontalAlignment: HorizontalAlignment.Fill
      }       
      contextActions: [
          ActionSet {                        
           // General SHARE Framework call
           // Will display all the SHARE Targets available
           InvokeActionItem {
               ActionBar.placement: ActionBarPlacement.InOverflow
                   query {
                       mimeType: "text/plain"
                       invokeActionId: "bb.action.SHARE"
                   }
                   onTriggered: {
                       data = Qt.myVKChatController.encodeQString(bodyLabel.text+qsTr("\nShared from imVK"));
                       console.log(data);
                   }
               }
         }
       ]      
  }     
  ImageView {
      imageSource: "asset:///icons/ic_location.amd"
      filterColor: ui.palette.textOnPlain
      horizontalAlignment: HorizontalAlignment.Right
      preferredHeight: ui.sdu(5.0)
      preferredWidth: preferredHeight
      visible: ListItemData.has_geo
      layoutProperties: StackLayoutProperties {
          spaceQuota: -1
      }
      onTouch: {
          if (event.isUp())
          {
              var google_url = "http://maps.google.com/?q=loc:%1,%2";              
              invokeQuery.mimeType = "text/html";
              var l_url = google_url.arg(ListItemData.geo.lattitude).arg(ListItemData.geo.longtitude);
              console.log("Setting invoke url:" + l_url);
              invokeQuery.setUri(l_url);
              invokeQuery.setData(l_url);
              invokeQuery.updateQuery();
              console.log("Invoke URI:"+ invokeQuery.uri);
              console.log("Invoke DATA:"+ invokeQuery.data);              
              console.log(ListItemData.geo.lattitude + " " + ListItemData.geo.longtitude);
//              invoke.trigger("bb.action.OPEN");
          }
      }
      attachedObjects:
      [         
              Invocation
              {
                  id: invoke
                  query: InvokeQuery
                  {                
                      id:invokeQuery
                      mimeType: "text/html"
    //                  uri: 
//                      invokeActionId: 
                      onQueryChanged: {
                          updateQuery();
                      }
                  }
                  onArmed: {
//                      if (invokeQuery.data != "") {
                          trigger("bb.action.OPEN");
//                      }
                  }    
                  
              }          
      ]
  }
}
