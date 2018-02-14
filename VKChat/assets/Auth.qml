import bb.cascades 1.4
import bb.system 1.2
import "utilities.js" as Utilities

    Container {       
        id: topAuthContainer 
        visible: false    
        ScrollView {
            
        accessibility.name: qsTr("Auth container")       
        WebView {
            id: authView
            accessibility.name: qsTr("Auth window")
            url: "https://oauth.vk.com/authorize?client_id={YOUR_TOKEN_HERE}&redirect_uri=https://oauth.vk.com/blank.html&scope=friends,photos,audio,wall,messages,notifications,offline&display=mobile&response_type=token"            
            onUrlChanged: {                
                console.log("Url changed:"+url);
                var params = Utilities.parseUrl(url);
                var i = url.toString().indexOf("access_token=");
                var err = url.toString().indexOf("error_description=");
                if (err > 0)
                {
                    myQmlToast.body = params["error_description"];
                    myQmlToast.show();
                }
                else
                if (i>0)
                {
                    console.log("Token found. Closing auth window");                    
                    console.log("User ID:"+params["user_id"]);
                    console.log("Token:"+params["access_token"]);
                    console.log("Expires in:"+params["expires_in"]);
                    myVKChatController.setAuthData(params["access_token"], params["user_id"], params["expires_in"]);
                    topAuthContainer.visible = false;                 
                }
                else 
                {
                    console.log("Token not found. Showing auth window");
                    topAuthContainer.visible = true;
    //                authSheet.open();
                    // Here we should popup oAuth window
    //                var newPage = oAuthDefinition.createObject();
    //                navigationPane.push(newPage);
                }
            }
            attachedObjects: [
                SystemToast {
                    id: myQmlToast
                    onFinished: {
//                        Application.quit();
                    }
                } ]
        }
    }
}