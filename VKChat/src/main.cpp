/*
 * Copyright (c) 2011-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>

#include <Qt/qdeclarativedebug.h>
#include "Loader.hpp"
#include "DataModel/MessageDataModel.h"

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
//    qInstallMsgHandler(debugOutputMessages);
    Application app(argc, argv);

    // Register the Loader class as new QML type
    qmlRegisterType<Loader>("QTLoader", 1, 0, "Loader");
    // Register own classes
    int type_id = qmlRegisterType<MessageDataModel>("VKChat", 1, 0, "MessageDataModel");
    qRegisterMetaType<MessageDataModel*>("MessageDataModel*");
    type_id = qmlRegisterType<Vkmessage>("VKChat", 1, 0, "Vkmessage");
    qRegisterMetaType<Vkmessage*>("Vkmessage*");
    type_id = qmlRegisterType<VKGeo>("VKGeo", 1, 0, "VKGeo");
    qRegisterMetaType<VKGeo*>("VKGeo*");
    type_id = qmlRegisterType<Vkattach>("VKChat", 1, 0, "Vkattach");
    qRegisterMetaType<Vkattach*>("Vkattach*");
    Q_UNUSED(type_id);

    // Create the Application UI object, this is where the main.qml file
    // is loaded and the application scene is set.
    ApplicationUI appui(&app);

    app.setAutoExit(true);

    // Enter the application main event loop.
    return Application::exec();
}
