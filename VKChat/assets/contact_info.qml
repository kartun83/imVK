import bb.cascades 1.4

Page {
    id: root
    
    titleBar: TitleBar {
        title: qsTr ("Contact Details")
    }
    
    Container {
        layout: DockLayout {}
        
        // The background image
//        BackgroundImage {
//            horizontalAlignment: HorizontalAlignment.Fill
//            verticalAlignment: VerticalAlignment.Fill
//        }
        
        AddressbookScrollView {
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top
                
                leftPadding: ui.du(15)
                topPadding: ui.du(50)
                rightPadding: ui.du(15)
                
                //! [0]
                ViewerField {
                    horizontalAlignment: HorizontalAlignment.Fill
                    title: qsTr("first name")
                    value: _addressBook.contactViewer.firstName
                }
                
                ViewerField {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topMargin: ui.du(5.5)
                    
                    title: qsTr("last name")
                    value: _addressBook.contactViewer.lastName
                }
                //! [0]
                
                ViewerField {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topMargin: ui.du(5.5)
                    
                    title: qsTr("birthday")
                    value: _addressBook.contactViewer.formattedBirthday
                }
                
                ViewerField {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topMargin: ui.du(5.5)
                    
                    title: qsTr("email")
                    value: _addressBook.contactViewer.email
                }
            }
        }
    }
    
    //! [1]
    actions: [
        ActionItem {
            title: qsTr ("Edit")
            imageSource: "asset:///images/action_editcontact.png"
            
            onTriggered: {
               // _addressBook.editContact()
               // navigationPane.push(contactEditor.createObject())
            }
        },
        DeleteActionItem {
            onTriggered: {
                //_addressBook.deleteContact()
                
                navigationPane.pop()
            }
        }
    ]
    //! [1]
    
    //! [2]
//    attachedObjects: [
//        ComponentDefinition {
//            id: contactEditor
//            source: "ContactEditor.qml"
//        }
//    ]
    //! [2]
}