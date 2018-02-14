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
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/system/CardDoneMessage>
#include <bb/platform/Notification>
#include <bb/device/DisplayInfo>
#include <bb/cascades/Menu>
#include <bb/cascades/ActionItem>
#include <bb/cascades/HelpActionItem>
#include <bb/platform/Notification>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <QTimer>

#include <bb/data/DataSource>

#include <SSmaatoAPI.h>
#include <SSmaatoAdView.h>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(),
        m_app(app),
        m_root(NULL),
        m_Settings("Kartun","imVKService",this),
        m_InvokeManager(new bb::system::InvokeManager(this)),
        m_HeadlessStart(false)
{
    bool connectResult;

    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning.
    Q_UNUSED(connectResult);
    connectResult = connect(m_InvokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(onInvoked(const bb::system::InvokeRequest&)));
    qDebug() << "HubIntegration: started and connected to invokeManager";


    // This is only available in Debug builds.
    Q_ASSERT(connectResult);

    switch(m_InvokeManager->startupMode()) {
        case bb::system::ApplicationStartupMode::LaunchApplication:
            qDebug() << "HeadlessHubIntegration: Regular application launch";
            break;
        case bb::system::ApplicationStartupMode::InvokeApplication:
            m_HeadlessStart = true;
            qDebug() << "--------------------- HeadlessHubIntegration: Launching app from invoke";
            break;
        case bb::system::ApplicationStartupMode::InvokeCard:
            m_HeadlessStart = true;
            m_isCard = true;
            qDebug() << "--------------------- HeadlessHubIntegration: Launching card from invoke";
            break;
        // enable when 10.3 beta is released
        //case ApplicationStartupMode::InvokeHeadless:
        case 4:
            qDebug() << "--------------------- HeadlessHubIntegration: Launching headless from invoke";
            m_HeadlessStart = true;
            break;
        default:
            qDebug() << "--------------------- HeadlessHubIntegration: other launch: " << m_InvokeManager->startupMode();
            break;
       }

    // ---------------------------------------------------------------------
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new bb::cascades::LocaleHandler(this);

    connectResult = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(connectResult);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(connectResult);

    // initial load
    onSystemLanguageChanged();

    qmlRegisterType<QTimer>("Lib.QTimer", 1, 0, "QTimer");

    // Set the default notification settings to allow instant previews
    bb::platform::NotificationDefaultApplicationSettings settings;
    settings.setPreview(bb::platform::NotificationPriorityPolicy::Allow);
    settings.apply();

    // Restore selected theme
    if (m_Settings.contains("app/Theme"))
    {
        VisualStyle::Type newStyle = (VisualStyle::Type)m_Settings.value("app/Theme").toInt();
        Application::instance()->themeSupport()->setVisualStyle(newStyle);
    }

//    if(!m_HeadlessStart) {

    // Register our classes for use in QML
    //int l_register = qmlRegisterType<kartun_vkchat::vk_chat_contoller>("kartun.vk_lib", 1, 0, "vk_chat_contoller");
    //Q_ASSERT( l_register );
    // Register the DataSource class as a QML type so that it's accessible in QML
    bb::data::DataSource::registerQmlTypes();

    // Banner stuff
    int type_id = qmlRegisterType<SSmaatoAdView>("smaatosdk", 1, 0, "SSmaatoAdView");
    qDebug() << "Registered SSmaatoAdView with type id:" << type_id;
    type_id = qmlRegisterType<SSmaatoAPI>("smaatosdk", 1, 0, "SSmaatoAPI");
    qDebug() << "Registered SSmaatoAPI with type id:" << type_id;

    // Make C++ classes available to QML.
    qmlRegisterType<Vkcontact>("VKChat", 1, 0, "Vkcontact");
    qRegisterMetaType<Vkcontact*>("Vkcontact*");
    qmlRegisterType<VKCommunity>("VKChat", 1, 0, "VKCommunity");
    qRegisterMetaType<VKCommunity*>("VKCommunity*");
    qmlRegisterType<VKWall>("VKChat", 1, 0, "VKWall");
    qRegisterMetaType<VKWall*>("VKWall*");
    qmlRegisterType<VKWallComment>("VKChat", 1, 0, "VKWallComment");
    qRegisterMetaType<VKWallComment*>("VKWallComment*");

    // Create instance for passing to QML

    this->_ChatController = new vk_chat_contoller();
    qDebug() << "Creating chat controller" << this->_ChatController;

    if(!m_HeadlessStart) {
    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
//    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    QmlDocument *qml = QmlDocument::create("asset:///VKTabbedPane.qml").parent(this);
//      QmlDocument *qml = QmlDocument::create("asset:///Splash.qml").parent(this);
//    Menu *menu = Menu::create()
//        .addAction(ActionItem::create().title("Menu Item 1"))
//        .addAction(ActionItem::create().title("Menu Item 2"))
//        .help(HelpActionItem::create());
//    Application::instance()->setMenu(menu);
    Application::instance()->setMenuEnabled(true);

    // React to application restore from minimized state
    // TODO :: May be unrequired later
    connectResult = QObject::connect(app, SIGNAL( awake() ), this->_ChatController, SLOT( onAwake() ));
    Q_ASSERT(connectResult);

    // Creates the property map and then exposes it for use in QML
    // All setContextProperty must be done prior to qml->createRootObject
    QDeclarativePropertyMap* propertyMap = new QDeclarativePropertyMap;
    qml->setContextProperty("propertyMap", propertyMap);
    qml->setContextProperty("myVKChatController", this->_ChatController);

//    qmlRegisterType<Vkmessage>("VKChat", 1, 0, "Vkmessage");
//    qRegisterMetaType<Vkmessage*>("Vkmessage*");
//    qmlRegisterType<VKmessage_attachment>("VKChat", 1, 0, "VKmessage_attachment");
//    qRegisterMetaType<VKmessage_attachment*>("VKmessage_attachment*");

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);
    bb::device::DisplayInfo display;
    QDeclarativePropertyMap* displayDimensions = new QDeclarativePropertyMap;
    displayDimensions->insert( "width", QVariant( display.pixelSize().width() ) );
    displayDimensions->insert( "height", QVariant( display.pixelSize().height() ) );
    qml->setContextProperty( "DisplayInfo", displayDimensions );

    // ------------------------------------------------------------
    // disconnect the UI from the headless, when the app is minimized.
    QTimer::singleShot(1000, this, SLOT(initReconnect()));

    }
}

void ApplicationUI::initReconnect() {
//    connect(m_app, SIGNAL(thumbnail()),  XMPP::get(), SLOT(disconnectToXMPPService()));
//    connect(m_app, SIGNAL(fullscreen()), XMPP::get(), SLOT(connectToXMPPService()));

        InvokeRequest request;
        request.setTarget("com.kartun.VKChatHeadlessService");
        request.setAction("com.kartun.VKChatHeadlessService.RESET");
        // Trigger background service
        m_InvokeManager->invoke(request);
//        Application::instance()->minimize();

}

void ApplicationUI::onInvoked(const bb::system::InvokeRequest& request)
{
    if(request.action().compare("bb.action.VIEW") == 0 || request.action().compare("bb.action.OPEN") == 0) {
    //         qDebug() << "HubIntegration: onInvoked: view item: " << request.data();
        this->_ChatController->setAuthData(m_Settings.value("vk_api/accountToken").toString(), m_Settings.value("vk_api/userID").toString(), 0);
        QmlDocument *qml = QmlDocument::create("asset:///Hub_Pane.qml").parent(this);
        JsonDataAccess jda;
        QVariantMap objectMap = (jda.loadFromBuffer(request.data())).toMap();
        qDebug() << "ApplicationUI::onInvoked - Invoke data:" << request.data();
        qDebug() << "ApplicationUI::onInvoked - Invoke data:" << objectMap;
        QVariantMap _attr = objectMap["attributes"].value<QVariantMap>();

        qDebug() << "User data:" << _attr.value("userData").toString();
        qDebug() << _attr;
        qml->setContextProperty("_app", this);
        qDebug() << "Setting context property myVKChatController";
        qml->setContextProperty("myVKChatController", this->_ChatController);

        // Starting IPC
        qDebug() << "UI :: Starting IPC";
        _ChatController->initIPC();
        _ChatController->setCurrentContactID(_attr.value("userData").toUInt());
        m_root = qml->createRootObject<NavigationPane>();
//        QmlDocument *_pageQml = QmlDocument::create("asset:///Hub_Chat.qml").parent(this);

                // Create root object for the UI
//        Page* _page = _pageQml->createRootObject<Page>();
//        AbstractPane *root = qml->createRootObject<AbstractPane>();
        Application::instance()->setScene(m_root);
//        m_root->push(_page);
//             JsonDataAccess jda;
//
//             QVariantMap objectMap = (jda.loadFromBuffer(request.data())).toMap();
//             QVariantMap itemMap = objectMap["attributes"].toMap();
//
//
//             QVariantList items = m_Settings.value("hub/items").toList();
//
//             QString addresse;
//             QString name;
//             QVariantMap item;
//             for(int index = 0; index < items.size(); index++) {
//                 item = items.at(index).toMap();
//                 QString sourceId = item["messageid"].toString();
//
//                  if (item["sourceId"].toString() == itemMap["messageid"].toString() ||
//                      item["sourceId"].toString() == itemMap["sourceId"].toString()) {
//
//                      addresse = item["userData"].toString();
//
//                      break;
//                  }
//             }
//
//             QmlDocument *qml = NULL;
//
//             QSettings settings("Kartun", "imVK");
////             if(settings.value("ConversationTheme", 0) == 0)
//             qml = QmlDocument::create("asset:///Hub_Chat.qml").parent(this);
////             else
////                 qml = QmlDocument::create("asset:///ConversationCard.qml")
////                                                               .parent(this);
//
//
//             // TODO :: Check
//             QDeclarativePropertyMap* propertyMap = new QDeclarativePropertyMap;
//             qml->setContextProperty("propertyMap", propertyMap);
//             qml->setContextProperty("myVKChatController", this->_ChatController);
//             qml->setContextProperty("_app", this);
//
//             if (m_root == NULL)
//             {
//                 qDebug() << "Empty m_root. Creating new";
//                 m_root = qml->createRootObject<NavigationPane>();
//             }
//             _page = qml->createRootObject<Page>();
//             Application::instance()->setScene(m_root);
//
//
//             QObject *thread = m_root->findChild<QObject*>("conversationCard");
//             if(thread != NULL) {
//
//
//                 bb::device::DisplayInfo display;
//                 QDeclarativePropertyMap* displayDimensions = new QDeclarativePropertyMap;
//                 displayDimensions->insert( "width", QVariant( display.pixelSize().width() ) );
//                 displayDimensions->insert( "height", QVariant( display.pixelSize().height() ) );
//                 qml->setContextProperty( "DisplayInfo", displayDimensions );
//
//                 thread->setProperty("name", item["name"].toString());
//
//                 if(QFile::exists(QDir::homePath() + QLatin1String("/vCards/") + addresse + ".png"))
//                     thread->setProperty("avatar", QDir::homePath() + QLatin1String("/vCards/") + addresse + ".png");
//                 else
//                     thread->setProperty("avatar", "asset:///images/avatar.png");
//
//                 thread->setProperty("room", false);
//                 thread->setProperty("id", addresse);
//
//
//             } else
//                 qDebug() << "pageThread variable is not found in the qml document :(";
//
//
//             InvokeRequest request;
//             request.setTarget("com.kartun.imVKService");
//             request.setAction("bb.action.MARKREAD");
//             request.setMimeType("hub/item");
//             request.setUri(QUrl("pim:"));
//
//             QByteArray bytes;
//             jda.saveToBuffer(objectMap, &bytes);
//             request.setData(bytes);
//
//             bb::platform::Notification *notif = new bb::platform::Notification();
//             notif->clearEffectsForAll();
//             notif->deleteLater();
//
//             // Pushing our page
//             qDebug() << "Pushing card:" << _page->toDebugString();
//             m_root->push(_page);

//             m_InvokeManager->invoke(request);

             // TODO :: Fix !!!
//             bool check = connect(XMPP::get(), SIGNAL(closeCardRequest()), this, SLOT(closeCard()));
//             Q_ASSERT(check);
//             Q_UNUSED(check);

        }

        if(request.action().compare("bb.action.COMPOSE") == 0) {

        }
}

void ApplicationUI::closeCard() {
    m_app->requestExit();

    if (m_isCard) {
        // Assemble response message
        CardDoneMessage message;
        message.setData(tr(""));
        message.setDataType("text/plain");
        message.setReason(tr("Success!"));

        // Send message
        m_InvokeManager->sendCardDone(message);
    }
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("imVK_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
