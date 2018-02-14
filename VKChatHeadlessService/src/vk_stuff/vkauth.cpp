/*
 * vkauth.cpp
 *
 *  Created on: May 10, 2015
 *      Author: Kartun
 */

#include "vkauth.hpp"
#include <bb/cascades/WebPage>
#include <bb/data/JsonDataAccess>
#include <QtCore/QDebug>
//#include <QtCore/QStringBuilder>

#include <bps/bps.h>
#include <bps/deviceinfo.h>

#include <QTime>

using namespace bb::data;
using namespace bb::network;

namespace kartun_vkchat
{

    vk_api::vk_api(QString accessToken, QString userId, int expiriesIn, QString appID, bool dontConnectLPS)
    {
        // TODO Auto-generated constructor stub
        Q_ASSERT(!accessToken.isEmpty());
        Q_ASSERT(!userId.isEmpty());
//        Q_ASSERT(expiriesIn); Could be zero for offline access
        this->_token  = accessToken;
        this->_userID = userId;
        this->_appID  = appID;
        this->_api_version = "5.40";
        this->_max_msg_id = "0";
        this->_lps_wait   = 25;
        _lps_request_active = false;

        QTime midnight(0,0,0);
        qsrand(midnight.secsTo(QTime::currentTime()));

        // Network related stuff
        // Create a QNetworkConfigurationManager object to manage
        // the network configurations and a QNetworkAccessManager to
        // find the name of the network interface

        m_pNetConfigMngr = new QNetworkConfigurationManager();
        m_pNetAccessMngr = new QNetworkAccessManager();
        Q_ASSERT(m_pNetAccessMngr);
        lps_NetworkManager = new QNetworkAccessManager();
        Q_ASSERT(lps_NetworkManager);

        // Set up a connection between the
        // QNetworkConfigurationManager's onlineStateChanged() signal
        // to the app's onOnlineStatusChanged(bool) slot. The state
        // indicates whether the device is online or offline

//        bool res = QObject::connect(m_pNetConfigMngr,
//                   SIGNAL(onlineStateChanged(bool)),
//                   this,
//                   SLOT(onOnlineStatusChanged(bool)));
//
//        Q_ASSERT(res);
//
//        // Set up a connection between the
//        // QNetworkConfigurationManager's configurationChanged()signal
//        // to the app's onConfigurationChanged() slot. The
//        // configuration indicates the type of connection that the
//        // device has (WLAN, 2G, 3G, and so on)
//
//        res = QObject::connect(m_pNetConfigMngr,
//              SIGNAL(configurationChanged(const QNetworkConfiguration &)),
//              this,
//              SLOT(onConfigurationChanged()));
//
//        Q_ASSERT(res);
        //// End of network related stuff

        // TODO :: Make proper
        Q_UNUSED(expiriesIn);
        //_timer = new QTimer(this);
        _url = "https://api.vk.com/method/";

        // Watchdog would wait for _lps_wait + 35 seconds. If lps replies within
        // this interval, watchdog would be restarted. Hopefully this would solve the issue with
        // message doubling on poor connections
        _lps_watchdog = new QTimer(this);
        _lps_watchdog->setSingleShot(true);
        _lps_watchdog->setInterval((_lps_wait + 35)*1000);

        perform_signal_connections();

        // Start PUSH service
        //startPushService();

        // Track visitors statistics
        vk_track_statistics();
        if (!dontConnectLPS)
        {
            // Connect to long poll server, so we would be able to receive notifications
            vk_connect_long_poll_server();
        }
        vk_get_my_info();
    }

    void vk_api::vk_set_online(bool isOnline)
    {
        QString _fetchUrl;
        if (isOnline == true)
        {
            _fetchUrl = QString("%1%2?%3&access_token=%4&v=%5").arg(_url, "account.setOnline","voip=0",_token,_api_version);
        }
        else
        {
            _fetchUrl = QString("%1%2?access_token=%3&v=%4").arg(_url, "account.setOffline",_token,_api_version);

        }

        // TODO :: Replace with native QUrl
        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(QUrl(_fetchUrl)));
        // TODO :: Add setting for this :)
        reply->setProperty("requestType", _request_setOnline);
    }

    void vk_api::vk_track_statistics()
    {
        QString _fetchUrl = QString("%1%3?&access_token=%2").arg(_url, _token, "stats.trackVisitor");

        qDebug() << _fetchUrl;

        // TODO :: Replace with native QUrl
        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(QUrl(_fetchUrl)));
        reply->setProperty("requestType", _request_trackStatistics);
    }

    vk_api::~vk_api()
    {
        // TODO Auto-generated destructor stub
    }

    void vk_api::vk_get_all_friends()
    {
        //QString _fetchUrl = QString("%1%3?&access_token=%2&%4&%5").arg(_url, _token, "friends.get",
        //"order=hints", "fields=nickname,photo_100,online,last_seen,status");
        if (!_url.isEmpty())
        {
            QString _fetchUrl = QString("%1%2").arg(_url, "friends.get");
            QUrl url(_fetchUrl);
            url.addQueryItem("access_token", _token );
            url.addQueryItem("order", "hints" );
            url.addQueryItem("fields", "nickname,photo_100,online,last_seen,status,has_mobile" );

            qDebug() << url;

            QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
            reply->setProperty("requestType", _request_getAllFriends);
        }
    }

    void vk_api::vk_get_dialogs()
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "messages.getDialogs");
        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        url.addQueryItem("count", QString::number(20) );
        url.addQueryItem("v", _api_version );

        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_getDialogs);
    }

    void vk_api::vk_get_wall(int wallID, bool isCommunity)
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "wall.get");
        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        if (isCommunity)
        {
            url.addQueryItem("owner_id", QString::number(-1 * wallID) );
        }
        else
        {
            url.addQueryItem("owner_id", QString::number(wallID) );
        }
        url.addQueryItem("count", QString::number(20) );
        url.addQueryItem("filter", "all" );
        url.addQueryItem("extended", QString::number(1) );
        url.addQueryItem("v", _api_version );

        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_getWall);
    }

    void vk_api::vk_get_groups()
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "groups.get");
        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        url.addQueryItem("extended", QString::number(1) );
        url.addQueryItem("filter", "admin, editor, moder, groups, publics" ); // , events
        url.addQueryItem("fields", "can_post,can_see_all_posts,status,can_create_topic,site" );
        url.addQueryItem("count", QString::number(20) );
        url.addQueryItem("v", _api_version );

        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_getGroups);
    }

    void vk_api::vk_get_online_friends()
    {

    }

    void vk_api::vk_get_all_messages()
    {
        // TODO :: Check !!!
        qCritical("WTF");
        QString _fetchUrl = QString("%1%2").arg(_url, "friends.get");
        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        url.addQueryItem("user_id", _userID);
        url.addQueryItem("order", "hints" );
        url.addQueryItem("fields", "nickname,photo_100,online,last_seen,status,has_mobile" );
        url.addQueryItem("v", _api_version );
        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_getAllMessages);
    }

    void vk_api::vk_get_messages_with_user(int userID)
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "messages.getHistory");
        QUrl url(_fetchUrl);
//        url.addQueryItem("rev", QString::number(1) );
//        url.addQueryItem("start_message_id", QString::number(-1) );
        url.addQueryItem("access_token", _token );
        url.addQueryItem("user_id", QString::number(userID) );
        url.addQueryItem("v", _api_version );
        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_getMessagesWithUser);
        reply->setProperty("userID", QString::number(userID));
    }

    void vk_api::getLongPollHistory(QString ts)
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "messages.getLongPollHistory");
        QUrl url(_fetchUrl);
        int actualTS = ts.toUInt() - 100;
//        url.addQueryItem("rev", QString::number(1) );
//        url.addQueryItem("start_message_id", QString::number(-1) );
        url.addQueryItem("access_token", _token );
        url.addQueryItem("ts", QString::number(actualTS) );
        if (this->_max_msg_id != 0) { url.addQueryItem("max_msg_id", _max_msg_id); }
        url.addQueryItem("v", _api_version );
        qDebug() << "messages.getLongPollHistory" << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_getLongPollHistory);
    }

    void vk_api::vk_send_message(int userID, QString messageText)
    {
//        QString _fetchUrl = QString("%1%2").arg(_url, "messages.send");
        QString _fetchUrl = QString("%1%2").arg(_url, "execute.sendMarkRead");

        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        url.addQueryItem("user_id", QString::number(userID) );
        url.addQueryItem("message", messageText );
        url.addQueryItem("guid", QString::number(qrand()) );
        url.addQueryItem("from_messageID", QString::number(0) );
        url.addQueryItem("v", _api_version );
//        url.addQueryItem("sticker_id", QString::number(2435));
        // TODO :: Add to settings, affects performance
        // lat
//        url.addQueryItem("lat", QString::number(qrand()) );
//        url.addQueryItem("long", QString::number(qrand()) );


        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_sendMessage);
    }

    void vk_api::perform_signal_connections()
    {
        bool connect_result;
        connect_result = connect(m_pNetAccessMngr, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(networkReplyReceived(QNetworkReply*)));
        Q_ASSERT(connect_result);

        connect_result = connect(_lps_watchdog, SIGNAL(timeout()),
                                   this, SLOT(lpsWatchdogTimeout()));
        Q_ASSERT(connect_result);

        connect_result = connect(lps_NetworkManager, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(onLPSReply(QNetworkReply*)));
        Q_ASSERT(connect_result);
//        connect_result = connect(m_pNetAccessMngr, SIGNAL(finished(QNetworkReply*)),
//                                this, SLOT(myInfoDownloaded(QNetworkReply*)));
//        Q_ASSERT(connect_result);
    }

    void vk_api::startPushService()
    {
        QString _fetchUrl = QString("%1%3?&access_token=%2").arg(_url, _token, "account.registerDevice");
        QUrl url(_fetchUrl);
        url.addQueryItem("device_model", deviceModel() );
        url.addQueryItem("device_id", deviceID() );
        url.addQueryItem("system_version", deviceVersion());
        //url.addQueryItem("settings", );
        qDebug() << url;



        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_registerDevice);

        //m_pushService = new PushService(_appID, INVOKE_TARGET_KEY_PUSH, this);//
//        m_reply = m_accessManager.get(QNetworkRequest(url));
//
//            // Connect to the reply finished signal.
//            connect(m_reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    }

    void vk_api::vk_connect_long_poll_server()
    {
        qDebug() << "imVK service :: Fetching new LPS server";
        QString _fetchUrl = QString("%1%3").arg(_url, "messages.getLongPollServer");
        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        url.addQueryItem("use_ssl", "1" );
        url.addQueryItem("need_pts", "1" );

        qDebug() << url;

        //m_pushService = new PushService(_appID, INVOKE_TARGET_KEY_PUSH, this);

        // Start watchdog
        _lps_watchdog->start();

        QNetworkReply* reply = lps_NetworkManager->get(QNetworkRequest(url));
        reply->setProperty("requestType", _lps_request_init);
    }

    void vk_api::onLPSReply(QNetworkReply* reply)
    {
//        QNetworkReply::NetworkError _errCode = reply->error();
        //QObject::dumpObjectInfo ();
        //qDebug() << QObject::receivers ( "onLPSReply" );

        _lps_request_active = false;
        if (reply->error() == QNetworkReply::NoError)
        {
            JsonDataAccess jda;
            QByteArray buffer = reply->readAll();
            // Notify that we received LPS reply
            emit networkLPSReceived(buffer);
            int requestType = reply->property("requestType").toInt();
            //uint fromID = reply->property("userID").toUInt();
            switch (requestType) {
               case _lps_request_init:  lps_initalized(buffer); break;
               case _lps_request_poll:  lps_replied(buffer); break;
               default: qFatal("Unknown request type for LPS fetch");
            }
            reply->deleteLater();
        }
        else
        {
            // There are problems. Restart watchdog
            _lps_watchdog->start();
            qDebug() << "imVK service :: Major LPS error, restarting watchdog" << reply->errorString();
            log_lps_error( reply );
        }
    }


    void vk_api::lps_initalized(QByteArray buffer)
    {
        // Load the JSON data
        JsonDataAccess jda;
        //QByteArray buffer = reply->readAll();

        QVariant list = jda.loadFromBuffer(buffer);
        QMap<QString,QVariant> map = list.value<QVariantMap>().value("response").toMap();
        qDebug() << map;

        this->_lps_data.key    = map.value("key").toString();
        this->_lps_data.server = map.value("server").toString();
        this->_lps_data.ts     = map.value("ts").toString();

        // LPS Initialized, so we can fetch updates
        vk_api::vk_fetch_long_poll();

//        reply->deleteLater();
    }

    void vk_api::vk_fetch_long_poll()
    {
        // TODO :: Add flag, so we wouldn't spam remote server with parallel listeners !!!!!
        QString _fetchUrl = QString("%1%2").arg("https://", this->_lps_data.server );
        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        url.addQueryItem("act", "a_check" );
        url.addQueryItem("key", this->_lps_data.key );
        url.addQueryItem("ts", this->_lps_data.ts );
        url.addQueryItem("wait", QString::number(_lps_wait) );
        url.addQueryItem("mode", "66" );
        if (this->_max_msg_id != 0) { url.addQueryItem("max_msg_id", _max_msg_id); }
        url.addQueryItem("v", _api_version );

        qDebug() << url;

        //m_pushService = new PushService(_appID, INVOKE_TARGET_KEY_PUSH, this);
//        bool connect_result;
//        Q_UNUSED(connect_result);   // Unused in release build
//        QNetworkAccessManager *manager2 = new QNetworkAccessManager(this);
//        connect_result = connect(manager2, SIGNAL(finished(QNetworkReply*)),
//                this, SLOT(lps_replied(QNetworkReply*)));
//        Q_ASSERT(connect_result);
        _lps_request_active = true;
        QNetworkReply* reply = lps_NetworkManager->get(QNetworkRequest(url));
        reply->setProperty("requestType", _lps_request_poll);
        vk_set_online(true);
    }

    void vk_api::vk_set_typing()
    {
        QString _fetchUrl = QString("%1%3?&access_token=%2").arg(_url, _token, "messages.setActivity");
        QUrl url(_fetchUrl);
        url.addQueryItem("user_id", _userID );
        url.addQueryItem("type", "typing" );
//        url.addQueryItem("peer_id", deviceVersion());
        //url.addQueryItem("settings", );
        qDebug() << url;



        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_startTyping);
    }

    void vk_api::vk_send_stiker(int userID, uint stickerID)
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "messages.send");
        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        url.addQueryItem("user_id", QString::number(userID) );
//        url.addQueryItem("chat_id", messageText );
        url.addQueryItem("guid", QString::number(qrand()) );
        url.addQueryItem("sticker_id", QString::number(stickerID));
        // TODO :: Add to settings, affects performance
        // lat
//        url.addQueryItem("lat", QString::number(qrand()) );
//        url.addQueryItem("long", QString::number(qrand()) );


        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_sendStiker);
    }

    void vk_api::vk_messages_mark_read(int userID, int startMessageID)
    {
        // TODO :: Add to settings, affects performance
        QString _fetchUrl = QString("%1%2").arg(_url, "messages.send");
        QUrl url(_fetchUrl);
        url.addQueryItem("access_token", _token );
        url.addQueryItem("peer_id", QString::number(userID));
        url.addQueryItem("start_message_id", QString::number(startMessageID) );

        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_markMessagesRead);
    }



    void vk_api::lps_replied(QByteArray buffer)
    {
        // Load the JSON data
        JsonDataAccess jda;
        qDebug() << "imVK service :: LPS Replied";
//        if ( reply->error() == QNetworkReply::NoError )
        {
            // TODO :: get new TS, parse updates and run
            //QByteArray buffer = reply->readAll();

            QVariant list = jda.loadFromBuffer(buffer);
            QMap<QString,QVariant> map = list.value<QVariantMap>();
            qDebug() << map;

            qDebug() << QString("Old ts:") << this->_lps_data.ts << QString("New TS:") << map.value("ts").toString();

            this->_lps_data.ts = map.value("ts").toString();

            int is_error = map.value("failed").toInt();

            if (is_error != 0)
            {
                qDebug() << "imVK serivce :: LPS Error" << is_error;
                switch (is_error) {
                    case 1:
                        qDebug() << "Event history expiried and/or lost";
                        break;
                    case 2:
                        qDebug() << "Long poll server key expiried!";

//                        break;
                    case 3:
                        vk_connect_long_poll_server();
//                        qDebug() << "Fucking shit! Zombie apocalypse!";
                        break;
                    default:
                        break;
                }
            }
            else
            {
                // This actually restarts watchgod timer, in case of network loss it would restore connection to LPS
                _lps_watchdog->start();
//                qDebug() << "Everything is ok :)";
            }

            QVariantList testList = list.value<QVariantMap>().value("updates").value<QVariantList>();
            QVariantMap entry;
            bool fullHistoryNeeded = false;
            QString l_attach_type;
            qDebug() << testList;
            // Seems this shit is bugged
            foreach(const QVariant &v, testList )
            {
                // Stop processing if we need full history
                if (fullHistoryNeeded) { break; }
                QVariantList singleResponse = v.value<QVariantList>();
//                foreach(const QVariant &element, singleResponse)
//                {
//                    qDebug() << element;
//                }
                int  userID = 0;
                uint flags = 0;
                uint timestamp = 0;
                QString message;
                QString subject;
                new_message_struct newMessage;
                switch (singleResponse.at(0).toInt()) {
                    case _lps_reply_messageDeleted:
                        qDebug() << "Message deleted";
                        break;
                    case _lps_reply_flagsChanged:
                        qDebug() << "Message flags changed";
                        break;
                    case _lps_reply_flagsSet:
                        qDebug() << "Message flags set";
                        break;
                    case _lps_reply_flagsReset:
                        qDebug() << "Message flags reset";
                        break;
                    case _lps_reply_newMessage:
                        // 4,$message_id,$flags,$from_id,$timestamp,$subject,$text,$attachments — добавление нового сообщения.
                        // Если сообщение отправлено в беседе, $from_id содержит id беседы + 2000000000.
                        // 0,1          ,2     ,3       ,4         ,5       ,6
                        // 4,$message_id,$flags,$from_id,$timestamp,$subject,$text,$attachments
//                        newMessage.out          =
                        flags = singleResponse.at(2).toUInt();
                        qDebug() << (flags & BIT1);
                        // TODO :: Confirm swap
                        if ((flags & BIT1) == 0) { newMessage.out = qlonglong(0);
                                                   newMessage.user_id      = _userID.toUInt();
                            } else {
                                                   newMessage.out = qlonglong(1);
                                                   newMessage.user_id      = singleResponse.at(3).toUInt();}
                        if ((flags & BIT0) == 0) { newMessage.read = qlonglong(0); } else { newMessage.read = qlonglong(1); }

                        newMessage.message_id   = singleResponse.at(1).toInt();
                        newMessage.from_id      = singleResponse.at(3).toUInt();
                        newMessage.timestamp    = singleResponse.at(4).toUInt();
                        newMessage.subject      = singleResponse.at(5).toString();
                        newMessage.text         = singleResponse.at(6).toString();

                        // TODO :: Parse attachments !!!
                        // TODO :: Parse flags !!!
                        // TODO :: Remove this dirty crutch
                        l_attach_type = singleResponse.at(7).value<QVariantMap>().value("attach1_type").toString();
                        qDebug() << "attachment type" << l_attach_type;
                        if ( (l_attach_type == "sticker") || ( l_attach_type.isEmpty()) )
                        {
                            emit newMessageReceived(newMessage, singleResponse.at(7));
                        }
                        else
                        {
                            // Since this shit provides attachments in stupid format, we have to request it properly
                            // TODO :: Finish
                            fullHistoryNeeded = true;
                            _max_msg_id = QString::number(newMessage.message_id);
                            getLongPollHistory(_lps_data.ts);
                            //emit newMessageReceived(newMessage, QVariant(QVariant::Invalid));
                        }

                        qDebug() << QString("Message received from %1. Timestamp: %4, Subject: %2, text:%3").arg(QString::number(userID), subject, message, QString::number(timestamp));
                        break;
                    case _lps_reply_incomingRead:
                        qDebug() << "Incoming messages read";
                        break;
                    case _lps_reply_outgoingRead:
                        qDebug() << "Outgoing messages read";
                        break;
                    case _lps_reply_userOnline:
                        userID = singleResponse.at(1).toInt();
                        flags  = singleResponse.at(2).toInt();
                        // TODO :: Check
                        emit userOnlineChanged((uint)abs(userID), true);
                        qDebug() << QString("User %1 become online").arg(userID);
                        break;
                    case _lps_reply_userOffline:
                        userID = singleResponse.at(1).toUInt();
                        flags  = singleResponse.at(2).toInt();
                        // TODO :: Check
                        emit userOnlineChanged((uint)abs(userID), false);
                        qDebug() << QString("User %1 become offline").arg(userID);
                        break;
                    case _lps_reply_chatChanged:
                        qDebug() << "Chat attributes changed";
                        break;
                    case _lps_reply_userTypingInDialog:
                        userID = singleResponse.at(1).toUInt();
                        flags  = singleResponse.at(2).toInt();
                        qDebug() << QString("User %1 typing in dialog").arg(userID);
                        break;
                    case _lps_reply_userTypingInChat:
                        userID = singleResponse.at(1).toUInt();
                        flags  = singleResponse.at(2).toInt();
                        qDebug() << QString("User %1 typing in chat").arg(userID);
                        break;
                    case _lps_reply_userCall:
                        qDebug() << "User made video-call";
                        break;
                    case _lps_reply_unreadChanged:
                        int count;
                        count = singleResponse.at(1).toInt();
                        qDebug() << QString("Amount of unread messages changed to %1").arg(count);
                        break;
                    default:
                        break;
                }
                qDebug() << singleResponse.at(0);

                //if (!v[0].isNull())
                {
//                    qDebug() << v.toMap().value("");

                    //
//                    entry["body"] = v.toMap().value("body").toString();
//                    entry["mid"] = v.toMap().value("mid");
//                    entry["uid"] = v.toMap().value("uid");
//                    entry["from_id"] = v.toMap().value("from_id");
//                    entry["read_state"] = v.toMap().value("read_state");
//                    entry["date"] = v.toMap().value("date");
//                    entry["out"] = v.toMap().value("out");
//                    //entry["photo_50"] = "none";
//                    m_chatModel->insert(entry);
                }
            }

            //list.value<QVariantList>();

            // As timestamp is updated, fetch again
            if (_lps_request_active == false)
            {
                vk_fetch_long_poll();
            }
        }

        //reply->deleteLater();
    }

    void vk_api::vk_get_my_info()
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "users.get");
        QUrl url(_fetchUrl);
        url.addQueryItem("user_ids", _userID );
        url.addQueryItem("fields", "photo_100,status" );
        url.addQueryItem("access_token", _token );
        url.addQueryItem("v", _api_version );
        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_myInfo);
    }

    void vk_api::vk_get_users_info(QStringList& users)
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "users.get");
        QUrl url(_fetchUrl);
        url.addQueryItem("user_ids", users.join(",") );
        url.addQueryItem("fields", "photo_100,status,online,last_seen,screen_name" );
        url.addQueryItem("access_token", _token );
        url.addQueryItem("v", _api_version );
        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_usersInfo);
    }

    void vk_api::vk_get_my_music()
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "audio.get");
        QUrl url(_fetchUrl);
//        url.addQueryItem("user_ids", _userID );
//        url.addQueryItem("fields", "photo_100" );
        url.addQueryItem("access_token", _token );
        url.addQueryItem("v", _api_version );
        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_myMusic);
    }

    void vk_api::vk_search_music(QString query)
    {
        QString _fetchUrl = QString("%1%2").arg(_url, "audio.search");
        QUrl url(_fetchUrl);
        url.addQueryItem("auto_complete", QString::number(1) );
        url.addQueryItem("sort", QString::number(2) );
        url.addQueryItem("count", QString::number(50) );
        url.addQueryItem("q", query );
        url.addQueryItem("access_token", _token );
        url.addQueryItem("v", _api_version );
        qDebug() << url;

        QNetworkReply* reply = m_pNetAccessMngr->get(QNetworkRequest(url));
        reply->setProperty("requestType", _request_myMusic);
    }

    void vk_api::networkReplyReceived(QNetworkReply *reply)
    {
        QNetworkReply::NetworkError _errCode = reply->error();

        if ( (_errCode != QNetworkReply::NoError) && ( _errCode != QNetworkReply::UnknownContentError) )
        {
            QString networkError = reply->errorString();
            log_lps_error(reply);
            emit networkErrorOccured(_errCode, networkError);
            return; // ??
        }

        JsonDataAccess jda;
        QByteArray buffer = reply->readAll();

        if (!jda.loadFromBuffer(buffer).value<QVariantMap>().value("error").isNull())
        {
            vk_parse_error(buffer);
        }
        QVariant list = jda.loadFromBuffer(buffer).value<QVariantMap>().value("response");

        int requestType = reply->property("requestType").toInt();
        uint fromID = reply->property("userID").toUInt();
        switch (requestType) {
            case _request_myInfo:
                    emit myInfoUpdated(list);
                    break;
            case _request_sendMessage:
                    // TODO :: May be additional info required, fetch message ID
                    emit messageSent(list);
                    break;
            case _request_setOnline:
                    break;  // No additional actions, currently
            case _request_trackStatistics:
                    break;  // No additional actions
            case _request_getAllFriends: emit contactListUpdated(list);
                    break;  // No additional actions, currently
            case _request_getAllMessages:
                    // TODO :: Check
//                    emit chatUpdated(list);
                    break;  // No additional actions, currently
            case _request_startTyping:
                    break;
            case _request_getGroups:
                    emit groupsUpdated(list);
                    break;
            case _request_sendStiker:
                    break;
            case _request_getLongPollHistory:
                    emit chatUpdatedLPS(list);
                    break;
            case _request_markMessagesRead:
                    break;
            case _request_getMessagesWithUser:
                    emit chatUpdated(fromID, list);
                    break;  // No additional actions, currently
            case _request_getDialogs:
                    emit dialogsUpdated(list); break;
            case _request_getWall:
                    emit wallDownloaded(list); break;
            case _request_myMusic:
                    emit musicUpdated(list); break;
            case _request_usersInfo:
                    emit usersInfoUpdated(list); break;
            case _request_registerDevice:
                    break;  // No additional actions
            default:
                qCritical("Unknown API request");
                break;
        }

        reply->deleteLater();
    }
    void vk_api::networkErrorReceived(QNetworkReply::NetworkError networkError, QString networkErrorStr)
    {
        //Q_ASSERT(_timer);
        //if (_timer->isActive() ) { _timer->stop(); }
        //_settings.online = false;
        emit networkErrorOccured(networkError, networkErrorStr);
    }

    void vk_api::vk_parse_error(const QByteArray &buffer)
    {
        // TODO :: Finish
//        emit networkErrorOccured(reply->error(), reply->errorString());
        QString workingDir = QDir::currentPath();
        QString _documentsPath  = QString("//" + workingDir + "/shared/documents/imVK");
        QFile   _lps_log_File(QString("%1/vk_api.log").arg(_documentsPath));

        bool result;
        result = _lps_log_File.open(QIODevice::Append);
//        qDebug() << _lps_log_File.error() << _lps_log_File.errorString();
        Q_ASSERT(result);
        if (result)
        {
//            QByteArray buffer = reply->readAll();
            QString _message = QString("%1 \n").arg(QTime::currentTime().toString(Qt::SystemLocaleLongDate));
            _lps_log_File.write(buffer);
            _lps_log_File.write("\n");
            _lps_log_File.close();
        }
    }

    void vk_api::message_parsed(QString message_id)
    {
        this->_max_msg_id = message_id;
    }

    void vk_api::lpsWatchdogTimeout()
    {
        // Timeout occured while polling LPS. Need to restart from scratch
        vk_connect_long_poll_server();
        //vk_fetch_long_poll();
    }

    QString vk_api::deviceModel() const
    {
        QString deviceModel;

            if (bps_initialize() == BPS_SUCCESS) {
                qDebug() << "bps initialized";
                deviceinfo_details_t *deviceDetails = 0;

                if (deviceinfo_get_details(&deviceDetails) == BPS_SUCCESS) {
                    deviceModel =  QString("%1 %2").arg(deviceinfo_details_get_model_name(deviceDetails), deviceinfo_details_get_model_number(deviceDetails));
                    deviceinfo_free_details(&deviceDetails);
                } else {
                    qDebug() << "error retrieving device details";
                }

                bps_shutdown();
            } else {
                qDebug() << "error initializing bps";
            }

            return deviceModel;
    }

    QString vk_api::deviceVersion() const
    {
        QString deviceVersion;

        if (bps_initialize() == BPS_SUCCESS) {
            qDebug() << "bps initialized";
            deviceinfo_details_t *deviceDetails = 0;

            if (deviceinfo_get_details(&deviceDetails) == BPS_SUCCESS) {
                deviceVersion = deviceinfo_details_get_device_os_version(deviceDetails);
                deviceinfo_free_details(&deviceDetails);
            } else {
                qDebug() << "error retrieving device details";
            }

            bps_shutdown();
        } else {
            qDebug() << "error initializing bps";
        }

        return deviceVersion;
    }

    QString vk_api::deviceID() const
    {
        QString deviceID;

        if (bps_initialize() == BPS_SUCCESS) {
            qDebug() << "bps initialized";
            deviceinfo_details_t *deviceDetails = 0;

            if (deviceinfo_get_details(&deviceDetails) == BPS_SUCCESS) {
                deviceID = deviceinfo_details_get_hardware_id(deviceDetails);
                //deviceinfo_free_identifying_details(&deviceDetails);
            } else {
                qDebug() << "error retrieving device details";
            }

            bps_shutdown();
        } else {
            qDebug() << "error initializing bps";
        }

        return deviceID;

    }

    void vk_api::unescapeXML(QString& inXML)
    {
//        "   &quot;
//        '   &apos;
//        <   &lt;
//        >   &gt;
//        &   &amp;
//        inXML.repl
        inXML.replace(QRegExp("&(amp|lt|gt);"),"\\1");
    }

    // When the onlineStateChanged() signal is emitted,
    // get the new online status (online or offline)

//    void vk_api::onOnlineStatusChanged(bool isOnline)
//    {
//        // Get the 'Online' status from the onlineStateChanged()
//        // signal
//
//        QString onlineStatus = isOnline ? "Online" : "Offline";
//
//        // Do something with the online status
//    }
//
//    // When the configurationChanged() signal is emitted, get the
//    // new network interface
//
//    void vk_api::onConfigurationChanged()
//    {
//        // Retrieve the interface (WLAN, 2G, and so on) being used
//        // to make the connection
//
//        QString intrfceNme =
//            m_pNetAccessMngr->activeConfiguration().bearerTypeName();
//
//        // Do something with the new interface name
//    }

    void vk_api::log_lps_error(QNetworkReply* reply)
    {
        emit networkErrorOccured(reply->error(), reply->errorString());
        QString workingDir = QDir::currentPath();
        QString _documentsPath  = QString("//" + workingDir + "/shared/documents/imVK");
        QFile   _lps_log_File(QString("%1/lps.log").arg(_documentsPath));

        bool result;
        result = _lps_log_File.open(QIODevice::Append);
        Q_ASSERT(result);
        if (result)
        {
            QByteArray buffer = reply->readAll();
            qDebug() << "imVK service :: LPS Error!!!:" << buffer << "," << reply->error() << reply->errorString();
            QString _message = QString("%1 : %2\n").arg(QTime::currentTime().toString(Qt::SystemLocaleLongDate),
                                                   reply->errorString());
            _lps_log_File.write(_message.toAscii());
            _lps_log_File.write(buffer);
            _lps_log_File.write("\n");
            _lps_log_File.close();
        }

        reply->deleteLater();

//        qDebug() <<  reply->errorString();
    }

    void vk_api::setNewToken(QString& accessToken)
    {
        _lps_watchdog->stop();
        _token = accessToken;
        vk_connect_long_poll_server();
    }


//    // TODO :: For HUB integration
//    void vk_api::onNewMessageReceived(vk_api::new_message_struct newMessage)
//    {
//
//    }


} /* namespace kartun_timetrack */
