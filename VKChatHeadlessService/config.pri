# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/Hub/HubAccount.cpp) \
        $$quote($$BASEDIR/src/Hub/HubCache.cpp) \
        $$quote($$BASEDIR/src/Hub/HubIntegration.cpp) \
        $$quote($$BASEDIR/src/Hub/UDSUtil.cpp) \
        $$quote($$BASEDIR/src/IPCServer.cpp) \
        $$quote($$BASEDIR/src/MyApplication.cpp) \
        $$quote($$BASEDIR/src/NetworkStatus.cpp) \
        $$quote($$BASEDIR/src/main.cpp) \
        $$quote($$BASEDIR/src/service.cpp) \
        $$quote($$BASEDIR/src/vk_stuff/VKmessageattachment.cpp) \
        $$quote($$BASEDIR/src/vk_stuff/vkauth.cpp) \
        $$quote($$BASEDIR/src/vk_stuff/vkmessage.cpp)

    HEADERS += \
        $$quote($$BASEDIR/src/Hub/HubAccount.hpp) \
        $$quote($$BASEDIR/src/Hub/HubCache.hpp) \
        $$quote($$BASEDIR/src/Hub/HubIntegration.hpp) \
        $$quote($$BASEDIR/src/Hub/UDSUtil.hpp) \
        $$quote($$BASEDIR/src/IPCServer.hpp) \
        $$quote($$BASEDIR/src/MyApplication.h) \
        $$quote($$BASEDIR/src/NetworkStatus.hpp) \
        $$quote($$BASEDIR/src/service.hpp) \
        $$quote($$BASEDIR/src/vk_stuff/VKmessageattachment.hpp) \
        $$quote($$BASEDIR/src/vk_stuff/vkauth.hpp) \
        $$quote($$BASEDIR/src/vk_stuff/vkmessage.hpp)
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
        $$quote($$BASEDIR/../src/Hub/*.c) \
        $$quote($$BASEDIR/../src/Hub/*.c++) \
        $$quote($$BASEDIR/../src/Hub/*.cc) \
        $$quote($$BASEDIR/../src/Hub/*.cpp) \
        $$quote($$BASEDIR/../src/Hub/*.cxx) \
        $$quote($$BASEDIR/../src/vk_stuff/*.c) \
        $$quote($$BASEDIR/../src/vk_stuff/*.c++) \
        $$quote($$BASEDIR/../src/vk_stuff/*.cc) \
        $$quote($$BASEDIR/../src/vk_stuff/*.cpp) \
        $$quote($$BASEDIR/../src/vk_stuff/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}.ts)