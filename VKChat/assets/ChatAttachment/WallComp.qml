import bb.cascades 1.4

Container {
    Label {
        text: "Wall."// Content type:" + ListItemData.contentType; 
    }
    Label {
        id: contentLabel
        text: qsTr("Wall attachments currently not supported") //ListItemData.body
        onCreationCompleted: {
            console.log("Wall link:" + ListItemData.body);
        }
    }
}
