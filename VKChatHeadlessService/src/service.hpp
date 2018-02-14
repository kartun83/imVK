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

#ifndef SERVICE_H_
#define SERVICE_H_

#include <QObject>
#include <QFileSystemWatcher>   // For monitoring of settings file changes
#include <bb/system/InvokeManager>
#include <bb/Application>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
//#include <QSettings>

#include "Hub/UDSUtil.hpp"
#include "Hub/HubIntegration.hpp"
#include "Hub/HubCache.hpp"
#include "NetworkStatus.hpp"
#include "./vk_stuff/vkauth.hpp"
#include "./vk_stuff/vkmessage.hpp"
//#include "VKmessageattachment.hpp"

#include "IPCServer.hpp"

namespace bb {
    class Application;
    namespace platform {
        class Notification;
    }
    namespace system {
        class InvokeManager;
        class InvokeRequest;
    }
}

using namespace kartun_vkchat;

class HeadlessHubIntegration;

class Service: public QObject
{
    Q_OBJECT;
public:
    Service(bb::Application *app);
    virtual ~Service() {}
    void initializeHub();
public Q_SLOTS:
    void chatListUpdatedLPS(const QVariant &list);
    void onNewMessageReceived(vk_api::new_message_struct newMessage, const QVariant &attachments);
        /**
         * Method hooked into the signal/slots mechanism, which gets invoked upon receiving fileChanged() signal from the settingsWatcher instance.
         */
    void settingsChanged(const QString & path);
private slots:
    void handleInvoke(const bb::system::InvokeRequest &);
    void onTimeout();
    void markHubItemRead(QVariantMap itemProperties);
    void markHubItemUnread(QVariantMap itemProperties);
    void removeHubItem(QVariantMap itemProperties);
    void onNetworkLPSReceived(const QByteArray &buffer);
//    void checkAccountAvailable();

private:
    void triggerNotification();

    bb::system::InvokeManager                       *m_InvokeManager;
    bb::Application                                 *m_app;
    UDSUtil                                         *m_UdsUtil;
    bb::platform::Notification                      *m_notify;
    QSettings                                       *m_Settings;
    HubCache                                        *m_HubCache;
    HubIntegration                                  *m_Hub;

    QMutex                                           m_InitMutex;
    NetworkStatus                                    m_NetworkStatus;
    // Keys for settings
    QString                                          c_vkapi_tokenKey;
    QString                                          c_vkapi_userIDKey;
    QString                                          c_vkapi_appIDKey;
    kartun_vkchat::vk_api                            *m_vkapi;
    uint                                             c_myID;
    // Watcher for qsettigns file changes
    QFileSystemWatcher* settingsWatcher;
    // Socket for IPC
    IPCServer                                        *m_ipcServer;
    QTcpSocket                                       *m_serverSocket;
    bool                                             m_apiConnected;

};

#endif /* SERVICE_H_ */
