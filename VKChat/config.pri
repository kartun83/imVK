# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            INCLUDEPATH += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK/public) \
                $$quote(${QNX_TARGET}/usr/include/bb/device) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtNetwork) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtXml) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtCore)

            DEPENDPATH += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK/public) \
                $$quote(${QNX_TARGET}/usr/include/bb/device) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtNetwork) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtXml) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtCore)

            LIBS += -lSmaatoSDKDevice \
                -lbbdevice \
                -lQtCore \
                -lQtXml \
                -lbbpim \
                -lQtNetwork

            LIBS += $$quote(-L/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK)

            PRE_TARGETDEPS += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK)

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            INCLUDEPATH += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK/public) \
                $$quote(${QNX_TARGET}/usr/include/bb/device) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtNetwork) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtXml) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtCore)

            DEPENDPATH += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK/public) \
                $$quote(${QNX_TARGET}/usr/include/bb/device) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtNetwork) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtXml) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtCore)

            LIBS += -lSmaatoSDKDevice \
                -lbbdevice \
                -lQtCore \
                -lQtXml \
                -lbbpim \
                -lQtNetwork

            LIBS += $$quote(-L/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK)

            PRE_TARGETDEPS += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK)

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK/public) \
                $$quote(${QNX_TARGET}/usr/include/bb/device) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtNetwork) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtXml) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtCore)

            DEPENDPATH += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK/public) \
                $$quote(${QNX_TARGET}/usr/include/bb/device) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtNetwork) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtXml) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtCore)

            LIBS += -lSmaatoSDKDevice \
                -lbbdevice \
                -lQtCore \
                -lQtXml \
                -lbbpim \
                -lQtNetwork

            LIBS += $$quote(-L/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK)

            PRE_TARGETDEPS += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK)

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK/public) \
                $$quote(${QNX_TARGET}/usr/include/bb/device) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtNetwork) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtXml) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtCore)

            DEPENDPATH += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK/public) \
                $$quote(${QNX_TARGET}/usr/include/bb/device) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtNetwork) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtXml) \
                $$quote(${QNX_TARGET}/usr/include/qt4/QtCore)

            LIBS += -lbbdevice \
                -lQtCore \
                -lQtXml \
                -lbbpim \
                -lSmaatoSDKSimulator \
                -lQtNetwork

            LIBS += $$quote(-L/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK)

            PRE_TARGETDEPS += $$quote(/home/kartun/Downloads/SOMA_SDK_for_BLACKBERRY_V1.1.3/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/Smaato-SmaatoSDK-BB10-v1.1.3-2014-07-02-A/SmaatoSDK)

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/AboutBox.qml) \
        $$quote($$BASEDIR/assets/ActiveCover.qml) \
        $$quote($$BASEDIR/assets/ActiveCoverSmall.qml) \
        $$quote($$BASEDIR/assets/AddressbookScrollView.qml) \
        $$quote($$BASEDIR/assets/Auth.qml) \
        $$quote($$BASEDIR/assets/Chat.qml) \
        $$quote($$BASEDIR/assets/ChatAttachment/AudioComp.qml) \
        $$quote($$BASEDIR/assets/ChatAttachment/BodyComponent.qml) \
        $$quote($$BASEDIR/assets/ChatAttachment/DocComp.qml) \
        $$quote($$BASEDIR/assets/ChatAttachment/ImageComp.qml) \
        $$quote($$BASEDIR/assets/ChatAttachment/VideoComp.qml) \
        $$quote($$BASEDIR/assets/ChatAttachment/WallComp.qml) \
        $$quote($$BASEDIR/assets/ChatHeaderElement.qml) \
        $$quote($$BASEDIR/assets/ChatItemElement.qml) \
        $$quote($$BASEDIR/assets/Communities.qml) \
        $$quote($$BASEDIR/assets/CommunityComments.qml) \
        $$quote($$BASEDIR/assets/CommunityCompose.qml) \
        $$quote($$BASEDIR/assets/CommunityView.qml) \
        $$quote($$BASEDIR/assets/Contacts.qml) \
        $$quote($$BASEDIR/assets/CustomTitleBannerControl.qml) \
        $$quote($$BASEDIR/assets/DialogItemElement.qml) \
        $$quote($$BASEDIR/assets/DialogsPage.qml) \
        $$quote($$BASEDIR/assets/EmotikonPicker.qml) \
        $$quote($$BASEDIR/assets/EqPresetListItem.qml) \
        $$quote($$BASEDIR/assets/Equalizer.qml) \
        $$quote($$BASEDIR/assets/Hub_Chat.qml) \
        $$quote($$BASEDIR/assets/Hub_Pane.qml) \
        $$quote($$BASEDIR/assets/Main2.qml) \
        $$quote($$BASEDIR/assets/MusicItemElement.qml) \
        $$quote($$BASEDIR/assets/MusicPage.qml) \
        $$quote($$BASEDIR/assets/PlayerControl.qml) \
        $$quote($$BASEDIR/assets/SettingsPage.qml) \
        $$quote($$BASEDIR/assets/Splash.qml) \
        $$quote($$BASEDIR/assets/TitlebarSeach.qml) \
        $$quote($$BASEDIR/assets/VKTabbedPane.qml) \
        $$quote($$BASEDIR/assets/ViewerField.qml) \
        $$quote($$BASEDIR/assets/WallHeaderElement.qml) \
        $$quote($$BASEDIR/assets/chat_bubble.qml) \
        $$quote($$BASEDIR/assets/contact_info.qml) \
        $$quote($$BASEDIR/assets/data/countries.xml) \
        $$quote($$BASEDIR/assets/data/emojies.xml) \
        $$quote($$BASEDIR/assets/data/eq.xml) \
        $$quote($$BASEDIR/assets/edit_name.qml) \
        $$quote($$BASEDIR/assets/icons/ic_addLike.png) \
        $$quote($$BASEDIR/assets/icons/ic_add_bookmarks.png) \
        $$quote($$BASEDIR/assets/icons/ic_attach.amd) \
        $$quote($$BASEDIR/assets/icons/ic_attach.png) \
        $$quote($$BASEDIR/assets/icons/ic_clear.png) \
        $$quote($$BASEDIR/assets/icons/ic_compose.png) \
        $$quote($$BASEDIR/assets/icons/ic_contact.png) \
        $$quote($$BASEDIR/assets/icons/ic_doctype_generic.amd) \
        $$quote($$BASEDIR/assets/icons/ic_doctype_generic.png) \
        $$quote($$BASEDIR/assets/icons/ic_doctype_music.amd) \
        $$quote($$BASEDIR/assets/icons/ic_doctype_music.png) \
        $$quote($$BASEDIR/assets/icons/ic_doctype_picture.amd) \
        $$quote($$BASEDIR/assets/icons/ic_doctype_picture.png) \
        $$quote($$BASEDIR/assets/icons/ic_doctype_video.amd) \
        $$quote($$BASEDIR/assets/icons/ic_doctype_video.png) \
        $$quote($$BASEDIR/assets/icons/ic_eq.png) \
        $$quote($$BASEDIR/assets/icons/ic_forward.amd) \
        $$quote($$BASEDIR/assets/icons/ic_forward.png) \
        $$quote($$BASEDIR/assets/icons/ic_groups.png) \
        $$quote($$BASEDIR/assets/icons/ic_info.png) \
        $$quote($$BASEDIR/assets/icons/ic_liked.amd) \
        $$quote($$BASEDIR/assets/icons/ic_liked.png) \
        $$quote($$BASEDIR/assets/icons/ic_location.amd) \
        $$quote($$BASEDIR/assets/icons/ic_location.png) \
        $$quote($$BASEDIR/assets/icons/ic_overflow_tab.png) \
        $$quote($$BASEDIR/assets/icons/ic_pause.amd) \
        $$quote($$BASEDIR/assets/icons/ic_pause.png) \
        $$quote($$BASEDIR/assets/icons/ic_pin.png) \
        $$quote($$BASEDIR/assets/icons/ic_play.amd) \
        $$quote($$BASEDIR/assets/icons/ic_play.png) \
        $$quote($$BASEDIR/assets/icons/ic_popular.png) \
        $$quote($$BASEDIR/assets/icons/ic_reload.png) \
        $$quote($$BASEDIR/assets/icons/ic_reply.amd) \
        $$quote($$BASEDIR/assets/icons/ic_reply.png) \
        $$quote($$BASEDIR/assets/icons/ic_search.amd) \
        $$quote($$BASEDIR/assets/icons/ic_search.png) \
        $$quote($$BASEDIR/assets/icons/ic_send.amd) \
        $$quote($$BASEDIR/assets/icons/ic_send.png) \
        $$quote($$BASEDIR/assets/icons/ic_settings.png) \
        $$quote($$BASEDIR/assets/icons/ic_share.png) \
        $$quote($$BASEDIR/assets/icons/ic_sort.png) \
        $$quote($$BASEDIR/assets/icons/ic_textmessage_dk.png) \
        $$quote($$BASEDIR/assets/icons/ic_to_bottom.png) \
        $$quote($$BASEDIR/assets/icons/ic_to_top.png) \
        $$quote($$BASEDIR/assets/icons/keyboard6.amd) \
        $$quote($$BASEDIR/assets/icons/keyboard6.png) \
        $$quote($$BASEDIR/assets/icons/smile.amd) \
        $$quote($$BASEDIR/assets/icons/smile.png) \
        $$quote($$BASEDIR/assets/img/ava_50.jpg) \
        $$quote($$BASEDIR/assets/img/icon_110.png) \
        $$quote($$BASEDIR/assets/img/offline.png) \
        $$quote($$BASEDIR/assets/img/online.png) \
        $$quote($$BASEDIR/assets/img/splat.png) \
        $$quote($$BASEDIR/assets/main.qml) \
        $$quote($$BASEDIR/assets/splash/logo_vk_1_576.png) \
        $$quote($$BASEDIR/assets/splash/splash.png) \
        $$quote($$BASEDIR/assets/splash/textura_bel_200.amd) \
        $$quote($$BASEDIR/assets/splash/textura_bel_200.png) \
        $$quote($$BASEDIR/assets/splash/textura_bel_400.amd) \
        $$quote($$BASEDIR/assets/splash/textura_bel_400.png) \
        $$quote($$BASEDIR/assets/statuses/blu.png) \
        $$quote($$BASEDIR/assets/statuses/otpravleno.png) \
        $$quote($$BASEDIR/assets/statuses/polucheno.png) \
        $$quote($$BASEDIR/assets/statuses/ser.png) \
        $$quote($$BASEDIR/assets/utilities.js)
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/DataModel/DialogDataModel.cpp) \
        $$quote($$BASEDIR/src/DataModel/MessageDataModel.cpp) \
        $$quote($$BASEDIR/src/Loader.cpp) \
        $$quote($$BASEDIR/src/VKCommunity.cpp) \
        $$quote($$BASEDIR/src/VKGeo.cpp) \
        $$quote($$BASEDIR/src/VKMusic.cpp) \
        $$quote($$BASEDIR/src/VKWall.cpp) \
        $$quote($$BASEDIR/src/VKWallComment.cpp) \
        $$quote($$BASEDIR/src/VKmessageattachment.cpp) \
        $$quote($$BASEDIR/src/Vkcontact.cpp) \
        $$quote($$BASEDIR/src/applicationui.cpp) \
        $$quote($$BASEDIR/src/main.cpp) \
        $$quote($$BASEDIR/src/vkauth.cpp) \
        $$quote($$BASEDIR/src/vkchatcontoller.cpp) \
        $$quote($$BASEDIR/src/vkmessage.cpp)

    HEADERS += \
        $$quote($$BASEDIR/src/DataModel/DialogDataModel.hpp) \
        $$quote($$BASEDIR/src/DataModel/MessageDataModel.h) \
        $$quote($$BASEDIR/src/Loader.hpp) \
        $$quote($$BASEDIR/src/VKCommunity.hpp) \
        $$quote($$BASEDIR/src/VKGeo.hpp) \
        $$quote($$BASEDIR/src/VKMusic.hpp) \
        $$quote($$BASEDIR/src/VKWall.hpp) \
        $$quote($$BASEDIR/src/VKWallComment.hpp) \
        $$quote($$BASEDIR/src/VKmessageattachment.hpp) \
        $$quote($$BASEDIR/src/Vkcontact.hpp) \
        $$quote($$BASEDIR/src/applicationui.hpp) \
        $$quote($$BASEDIR/src/vkauth.hpp) \
        $$quote($$BASEDIR/src/vkchatcontoller.hpp) \
        $$quote($$BASEDIR/src/vkmessage.hpp)
}

CONFIG += precompile_header

PRECOMPILED_HEADER = $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../src/.PCMReports/*.c) \
        $$quote($$BASEDIR/../src/.PCMReports/*.c++) \
        $$quote($$BASEDIR/../src/.PCMReports/*.cc) \
        $$quote($$BASEDIR/../src/.PCMReports/*.cpp) \
        $$quote($$BASEDIR/../src/.PCMReports/*.cxx) \
        $$quote($$BASEDIR/../src/DataModel/*.c) \
        $$quote($$BASEDIR/../src/DataModel/*.c++) \
        $$quote($$BASEDIR/../src/DataModel/*.cc) \
        $$quote($$BASEDIR/../src/DataModel/*.cpp) \
        $$quote($$BASEDIR/../src/DataModel/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs) \
        $$quote($$BASEDIR/../assets/ChatAttachment/*.qml) \
        $$quote($$BASEDIR/../assets/ChatAttachment/*.js) \
        $$quote($$BASEDIR/../assets/ChatAttachment/*.qs) \
        $$quote($$BASEDIR/../assets/data/*.qml) \
        $$quote($$BASEDIR/../assets/data/*.js) \
        $$quote($$BASEDIR/../assets/data/*.qs) \
        $$quote($$BASEDIR/../assets/icons/*.qml) \
        $$quote($$BASEDIR/../assets/icons/*.js) \
        $$quote($$BASEDIR/../assets/icons/*.qs) \
        $$quote($$BASEDIR/../assets/img/*.qml) \
        $$quote($$BASEDIR/../assets/img/*.js) \
        $$quote($$BASEDIR/../assets/img/*.qs) \
        $$quote($$BASEDIR/../assets/splash/*.qml) \
        $$quote($$BASEDIR/../assets/splash/*.js) \
        $$quote($$BASEDIR/../assets/splash/*.qs) \
        $$quote($$BASEDIR/../assets/statuses/*.qml) \
        $$quote($$BASEDIR/../assets/statuses/*.js) \
        $$quote($$BASEDIR/../assets/statuses/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}_ru.ts) \
    $$quote($${TARGET}.ts)
