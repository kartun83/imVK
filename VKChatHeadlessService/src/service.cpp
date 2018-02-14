/*
 * Copyright (c) 2013-2015 BlackBerry Limited.
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

#include "service.hpp"

#include <bb/Application>
#include <bb/platform/Notification>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <bb/system/InvokeManager>

#include <bb/data/JsonDataAccess>

#include <QTimer>

//#include "Hub/HubCache.hpp"


using namespace bb::platform;
using namespace bb::system;
using namespace bb::data;
using namespace kartun_vkchat;

Service::Service(bb::Application *app) :
        QObject(app),
        m_InvokeManager(new bb::system::InvokeManager()),
        m_app(app),
        m_UdsUtil(NULL),
        m_notify(new Notification(this)),
        m_Settings(NULL),
        m_HubCache(NULL),
        m_Hub(NULL),
        c_vkapi_tokenKey("vk_api/accountToken"),
        c_vkapi_userIDKey("vk_api/userID"),
        c_vkapi_appIDKey("vk_api/applicationID"),
        m_apiConnected(false)

{
    m_InvokeManager->setParent(this);
//    m_InvokeManager->connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
//            this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));
    qDebug() << "-----------------------------------\nStart Headless app!...\n------------------------------------";


//    OTRL_INIT;


    qDebug() << "initializeHub()";
    initializeHub();

    NotificationDefaultApplicationSettings settings;
    settings.setPreview(NotificationPriorityPolicy::Allow);
    settings.apply();

    m_notify->setTitle("imVK Service");
    m_notify->setBody("imVK service started");

    bb::system::InvokeRequest request;
    request.setTarget("com.kartun.VKChatHeadlessService");
    request.setAction("bb.action.START");
    m_notify->setInvokeRequest(request);

    m_ipcServer = new IPCServer(this);

    onTimeout();
}

void Service::handleInvoke(const bb::system::InvokeRequest & request)
{
    qDebug() << "imVK service:: handleInvoke";
    if (request.action().compare("com.kartun.VKChatHeadlessService.RESET") == 0) {
        triggerNotification();
    }
}

void Service::triggerNotification()
{
    // Timeout is to give time for UI to minimize
    QTimer::singleShot(2000, this, SLOT(onTimeout()));
}

void Service::onTimeout()
{
    Notification::clearEffectsForAll();
    Notification::deleteAllFromInbox();
    m_notify->notify();

    // HUB initialized, check if there are required data for vk_api
    m_Settings = new QSettings("Kartun", "imVKService");
    // Force the creation of the settings file so that we can watch it for changes.
    m_Settings->sync();
    settingsWatcher = new QFileSystemWatcher(this);
    settingsWatcher->addPath(m_Settings->fileName());
    connect(settingsWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(settingsChanged(const QString&)));
    settingsChanged("test");
//    QTimer::singleShot(5000, this, SLOT(checkAccountAvailable()));
}

void Service::markHubItemRead(QVariantMap itemProperties) {

    qint64 itemId;

    if (itemProperties["sourceId"].toString().length() > 0) {
        itemId = itemProperties["sourceId"].toLongLong();
    } else if (itemProperties["messageid"].toString().length() > 0) {
        itemId = itemProperties["messageid"].toLongLong();
    }

    m_Hub->markHubItemRead(m_Hub->categoryId(), itemId);
}

void Service::markHubItemUnread(QVariantMap itemProperties) {

    qint64 itemId;

    if (itemProperties["sourceId"].toString().length() > 0) {
        itemId = itemProperties["sourceId"].toLongLong();
    } else if (itemProperties["messageid"].toString().length() > 0) {
        itemId = itemProperties["messageid"].toLongLong();
    }

    m_Hub->markHubItemUnread(m_Hub->categoryId(), itemId);
}

void Service::removeHubItem(QVariantMap itemProperties) {

    qint64 itemId;
    if (itemProperties["sourceId"].toString().length() > 0) {
        itemId = itemProperties["sourceId"].toLongLong();
    } else if (itemProperties["messageid"].toString().length() > 0) {
        itemId = itemProperties["messageid"].toLongLong();
    }

    m_Hub->removeHubItem(m_Hub->categoryId(), itemId);
}


void Service::initializeHub() {
//    return;

    m_InitMutex.lock();

    // initialize UDS
    if (!m_UdsUtil) {
        qDebug() << "new UDSUtil()";
        m_UdsUtil = new UDSUtil(QString("VKChatHeadlessService"), QString("hubassets"));
    }

    if (!m_UdsUtil->initialized()) {
        qDebug() << "initialize()";
        m_UdsUtil->initialize();
    }

    if (m_UdsUtil->initialized() && m_UdsUtil->registered()) {
        qDebug() << "m_UdsUtil->initialized() && m_UdsUtil->registered()";
        qDebug() << "Fix this!!!!!!";
        if (!m_Settings) {

            m_Settings = new QSettings("Kartun", "imVKService");
        }
        if (!m_HubCache) {
            m_HubCache = new HubCache(m_Settings);
        }
        if (!m_Hub) {
            qDebug() << "new HubIntegration()";
            m_Hub = new HubIntegration(m_UdsUtil, m_HubCache);
        }
    }

    qDebug() << "done initializeHub()";
    m_InitMutex.unlock();
}

//void Service::checkAccountAvailable()
void Service::settingsChanged(const QString& path)
{
    qDebug() << "Service::settingsChanged :: Detected settings file changed";
    if (!m_Settings) {
        m_Settings = new QSettings("Kartun", "imVKService");
    }

    m_Settings->sync();
    qDebug() << m_Settings->value(c_vkapi_tokenKey).toString();
    if (m_Settings->contains(c_vkapi_tokenKey))
    {
        QString l_appid = m_Settings->value(c_vkapi_appIDKey).toString();
        QString l_token = m_Settings->value(c_vkapi_tokenKey).toString();
        QString l_userid = m_Settings->value(c_vkapi_userIDKey).toString();
        c_myID = m_Settings->value(c_vkapi_userIDKey).toUInt();
        qDebug() << "Found settings for headless process, starting vk_api headless";
        qDebug() << "Token" << l_token;
        m_vkapi = new kartun_vkchat::vk_api(l_token, l_userid, 0, l_appid, false);
//        m_vkapi = new kartun_vkchat::vk_api(l_token, l_userid, 0, l_appid, true);
        bool connect_result = connect(m_vkapi, SIGNAL(chatUpdatedLPS(QVariant)),
                                      this, SLOT(chatListUpdatedLPS(QVariant)));
        Q_ASSERT(connect_result);
        if (!m_apiConnected)
        {
            connect_result = QObject::connect(m_vkapi, SIGNAL(newMessageReceived(vk_api::new_message_struct, QVariant)),
                                          this, SLOT(onNewMessageReceived(vk_api::new_message_struct, QVariant)));
            Q_ASSERT(connect_result);
            Q_UNUSED(connect_result);

            connect_result = QObject::connect(m_vkapi, SIGNAL(networkLPSReceived(QByteArray)),
                                                      this, SLOT(onNetworkLPSReceived(QByteArray)));
            Q_ASSERT(connect_result);
            Q_UNUSED(connect_result);
        }
        else
        {
            qDebug() << "imVK service:: setting new token :" << l_token;
            m_vkapi->setNewToken(l_token);
        }
        m_apiConnected = true;
//        qDebug() << "imVK Service :: Disconnect settings watcher";
//        connect_result = disconnect(settingsWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(settingsChanged(const QString&)));
//        Q_ASSERT(connect_result);
//        Q_UNUSED(connect_result);
    }
    else
    {
        // Still need to wait for settings
        qDebug() << "Headless :: No token found. Waiting for token";
//        m_Settings = new QSettings("Kartun", "imVKService");
//        qDebug() << m_Settings;
//        QTimer::singleShot(5000, this, SLOT(checkAccountAvailable()));
    }
}

void Service::chatListUpdatedLPS(const QVariant& list)
{
    QVariantList testList = list.value<QVariantMap>().value("messages").value<QVariantMap>().value("items").value<QVariantList>();
                   qDebug() << "Parsing message list from LPS";
                   QVariantMap entry;
                   bool messageInserted = false;
//                   uint prevLatestMessageTimestamp = m_latestMessage->getTimestamp();

                   // Seems this shit is bugged
                   QListIterator<QVariant> i(testList);
                   i.toBack();

                   while (i.hasPrevious())
                   {
                       const QVariant &v = i.previous();
                       if (!v.toMap().value("body").isNull())
                       {
           //                qDebug() << v.toMap().value("body");

                           // TODO :: Remove
                           QMap<QString, QVariant> responseMap = v.toMap();
                           qDebug() << responseMap;
           //
                           uint l_fromID    = responseMap.value("user_id").toUInt();
   //                        uint _userID    = responseMap.value("user_id").toUInt();
   //                        uint _timestamp = responseMap.value("date").toUInt();
                           int  _messageID = responseMap.value("id").toInt();
           //                bool _out       = responseMap.value("out").toBool();
           //

                           kartun_vkchat::Vkmessage* newMessage2 = new Vkmessage(v, c_myID, this);
                           // (qint64 categoryId, QVariantMap &itemMap, QString name, QString subject,
//                               qint64 timestamp, QString itemSyncId,  QString itemUserData, QString itemExtendedData, bool notify)
                           QVariantMap map;
                           qint64 l_actualTimestamp = (quint64)(newMessage2->getTimestamp()) * 1000;
//                           m_Hub->updateHubItem()
                           map["userID"] = newMessage2->getOut() ? QString::number(newMessage2->getFromID()) : QString::number(newMessage2->getUserID());
                           map["messageID"] = _messageID;
                           m_Hub->addHubItem((quint64)1, map, map["userID"].toString(), newMessage2->getText(), l_actualTimestamp,
                                             QString::number(newMessage2->getMessageID()), map["userID"].toString(), QString("another ???"), true);
//                                             (bool)newMessage2->getReadState());


                       }
                   }
                   if (messageInserted)
                   {
   //                    // display notification
   //                    Notification* pNotification = new Notification();
   //
   //                    pNotification->setTitle(QString("%1 %2").arg(m_latestMessage->getFirstName(), m_latestMessage->getLastName()));
   //                    pNotification->setBody(m_latestMessage->getText());
   //                    pNotification->setTimestamp(m_latestMessage->getTimestamp());
   //                    pNotification->setIconUrl(QUrl(QString("file://%1/app/native/assets/img/icon_110.png").arg(QDir::currentPath())));
   //
   //                    NotificationError::Type result = pNotification->notify();
   //                    qDebug() << "Notification result" << result;
                       // TODO :: Temporary
//                       emit chatUpdated(m_chatModel2);
//                       emit latestMessageUpdated(m_latestMessage);
                   }

//                   m_ipcServer->write(test);
   //                onLatestMessageUpdated(m_latestMessage);
//                   if (prevLatestMessageTimestamp < m_latestMessage->getTimestamp())
//                       onLatestMessageUpdated(m_latestMessage);
}

void Service::onNewMessageReceived(vk_api::new_message_struct newMessage,
        const QVariant& attachments)
{
            qDebug() << "imVK service:: onNewMessageReceived";
    //        bb::cascades::GroupDataModel* t_chatModel;  // Temporary data model
            // We could come here, if no chat have been downloaded, but some contact sent us a message
            // Depending on direction, put in proper cache
            // TODO :: Confirm
            uint l_user_id = newMessage.from_id;
//            if (newMessage.out) { l_user_id = newMessage.user_id; }
//    //        if (!this->m_global_chat_container->value(newMessage.from_id) )
//            if (!this->m_global_chat_container->value(l_user_id) )
//            {
//
//                // No history yet.  Need to create new container
//                // TODO :: Refactor
//                QMap<uint, Vkmessage*>* _newCache = new QMap<uint, Vkmessage*>();
//                this->m_global_chat_container->insert(l_user_id, _newCache);
//            }

            // TODO :: Add notification if app is minimized !!
            // TODO :: Test
            //t_chatModel->insert(entry);

            Vkmessage* newMessage2 = new Vkmessage(newMessage, c_myID, attachments, this);

            QVariantMap map;
            map["userID"] = newMessage2->getOut() ?  QString::number(newMessage2->getUserID()) : QString::number(newMessage2->getFromID());
            map["messageID"] = QString::number(newMessage2->getMessageID());
            qint64 l_actualTimestamp = (quint64)(newMessage2->getTimestamp()) * 1000;
            m_Hub->addHubItem((quint64)1, map, QString::number(newMessage2->getFromID()), newMessage2->getText(), l_actualTimestamp,
                              QString::number(newMessage2->getMessageID()), map["userID"].toString(), QString("another ???"), true);

//            qDebug() << "imVK headless:: Preparing to forward LPS reply";
//            //    JsonDataAccess jda;
//            //    QByteArray* buffer = new QByteArray();
//            //    jda.saveToBuffer(list, buffer);
//            qDebug() << "Sending bytes:" << buffer.size();
//            //                   QByteArray test("abc");
//            m_ipcServer->write(buffer);


//            if (!newMessage2->getOut()) { m_latestMessage = newMessage2; }
//    //        uint _fromID = newMessage2->getFromID();
//            uint _fromID = l_user_id;
//
//            // Store in global cache. If value is there, it would be overwritten, otherwise added
//            m_global_chat_container->value(_fromID)->insert(newMessage2->getMessageID(),newMessage2);
//
//    //        qDebug() << _fromID << " userid:" << newMessage2->getUserID() << "fromid:" << newMessage2->getFromID();
//            // Update in dialogs as well
//
//            m_dialogsModel->replaceMessage(_fromID, newMessage2);
//
//
//            // If this message relevant for the currently displayed chat, update data model
//            if (_fromID == _currentContactID)
//            {
//                QVariantList match = m_chatModel->findExact(newMessage2);
//
//                // TODO :: Check with my datamodel
//                m_chatModel2->insertItem(newMessage2);
//
//                // In global contacts cache there is own info, that isn't required in contact list
//                if (!match.isEmpty())
//                {
//                    bool result = m_chatModel->updateItem(match, newMessage2);
//                    Q_ASSERT(result);
//                    Q_UNUSED(result);
//                }
//                else
//                {
////                    m_chatModel->insert(newMessage2);
//                }
//                // This notifies view that new items arrived
////                emit chatUpdated(m_chatModel2);
//            }

    //        t_chatModel->insert(newMessage2);
    //        m_global_messageCache->insert(newMessage2->getMessageID(), newMessage2);

    //        emit chatUpdated2(t_chatModel);
//            emit message_parsed(QString::number(newMessage.message_id));
//            emit latestMessageUpdated(m_latestMessage);
}

void Service::onNetworkLPSReceived(const QByteArray& buffer)
{
    qDebug() << "imVK headless:: Preparing to forward LPS reply";
//    JsonDataAccess jda;
//    QByteArray* buffer = new QByteArray();
//    jda.saveToBuffer(list, buffer);
    QByteArray newBA(buffer);
    newBA.prepend("[LPS]");
    qDebug() << "Sending bytes:" << newBA.size();
//                   QByteArray test("abc");
    m_ipcServer->write(buffer);
}
