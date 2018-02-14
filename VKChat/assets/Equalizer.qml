import bb.cascades 1.4
import VKChat 1.0

Page {
    Container {
        ListView {
            id: eqPresets
            property int activePreset : Qt.myVKChatController.mediaPlayer.equalizerPreset
            accessibility.name: qsTr("List of available EQ presets") + Retranslate.onLanguageChanged
            dataModel: XmlDataModel {
                source: "asset:///data/eq.xml"
            }
            listItemComponents: ListItemComponent {
                type: "item"
                EqPresetListItem {
//                    isActive: ListItemData.value ==  ? true : false
                    isActive: ListItemData.value == Qt.eqPresets.activePreset ? true : false
                    name: ListItemData.name
//                        isActive: fal
                    onCreationCompleted: {
                        Qt.eqPresets.activePresetChanged.connect(updateHighlight);
                    }
                    function updateHighlight()
                    {
                        isActive : ListItemData.value == Qt.eqPresets.activePreset ? true : false
                        console.log("Setting active preset:" + Qt.eqPresets.activePreset);
                    }
                }
            }
            
            onCreationCompleted: {
                Qt.myVKChatController = myVKChatController;
                Qt.eqPresets          = eqPresets;
            }
            onTriggered: {
                    var preset = dataModel.data(indexPath);                    
                    console.log("Setting preset:"+preset.value);
                    var result = Qt.myVKChatController.mediaPlayer.setEqualizerPreset(preset.value);
                    console.log("Result of setting EQ preset:" + result);
                    console.log("Current EQ preset:"+Qt.myVKChatController.mediaPlayer.equalizerPreset);                   
            }
        }
    }
}
