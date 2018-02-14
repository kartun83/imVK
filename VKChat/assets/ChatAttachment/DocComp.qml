import bb.cascades 1.4

Container {
    Label {
        text: "Document"
    }
    Label {
        id: contentLabel
        text: qsTr("Document attachments currently not supported") //ListItemData.body
        onCreationCompleted: {
            console.log("Document URL:" + ListItemData.body);
        }
    }
}
