/*
 * vkchatcontoller.cpp
 *
 *  Created on: May 18, 2015
 *      Author: Kartun
 */

#include "vkchatcontoller.hpp"
#include <QFile>
#include <functional>
#include <bb/system/SystemToast>
#include "VKmessageattachment.hpp"

#include <bb/platform/Notification>
#include <bb/platform/NotificationDefaultApplicationSettings>

#include <bb/system/InvokeRequest>

using namespace bb::cascades;
using namespace bb::multimedia;
using namespace bb::system;
using namespace bb::platform;

namespace kartun_vkchat
{

    vk_chat_contoller::vk_chat_contoller(QObject* parent) :
            QObject(parent) ,
            m_invokeManager(new InvokeManager(this)),
            m_port(5483),
            m_clientSocket(new QTcpSocket(this))
    {
        vk_app_id = "PUT_YOUR_APP_ID_HERE";
        m_authorized = false;
        m_playlistCounter = 0;
        m_Settings = new QSettings("Kartun", "imVKService", this);
        getPaths();
        // TODO Auto-generated constructor stub
        m_contactsModel = new GroupDataModel(this);
        //m_contactsModel = new ArrayDataModel(this);
        Q_ASSERT(m_contactsModel);
        m_contactsModel->setGrouping(ItemGrouping::ByFirstChar);
        QStringList keys;
        //keys << "online" << "last_seen" << "last_name" << "first_name";
        keys << "last_name" << "first_name";
        m_contactsModel->setSortingKeys(keys);
        m_contactsModel->setSortedAscending(true);

        m_currentContact = NULL;    // Currently no current contact :) obviously
        m_myInfo         = NULL;
        m_currentCommunity = NULL;
        vk_api::new_message_struct newMessage;
        newMessage.text = QString("No updates yet");
        newMessage.timestamp = 0; newMessage.from_id = 0; newMessage.out = false;
        m_latestMessage  = new Vkmessage(newMessage, 0, QVariant(QVariant::Invalid), this);    // And no message atmo

        keys.clear();
        m_chatModel = new GroupDataModel(this);
        Q_ASSERT(m_chatModel);
        keys << "date" << "message_id" << "contentType";
        m_chatModel->setSortingKeys(keys);
        m_chatModel->setSortedAscending(true);
        QStringList keys2;

        m_musicModel = new GroupDataModel(this);
        Q_ASSERT(m_musicModel);
        m_musicModel->setGrouping(ItemGrouping::None);

        keys.clear();
        m_wallModel = new GroupDataModel(this);
        Q_ASSERT(m_wallModel);
        keys << "date";
        m_wallModel->setSortingKeys(keys);
//        m_wallModel->setSortedAscending(false);
        m_wallModel->setGrouping(ItemGrouping::ByFullValue);
//        m_wallModel->setGrouping(ItemGrouping::None);

        m_wallCommentsModel = new GroupDataModel(this);
        Q_ASSERT(m_wallCommentsModel);
        m_wallCommentsModel->setSortingKeys(keys);
        m_wallCommentsModel->setSortedAscending(false);
        m_wallCommentsModel->setGrouping(ItemGrouping::ByFullValue);


        m_chatModel2 = new MessageDataModel(this);
        Q_ASSERT(m_chatModel2);

//        m_dialogsModel = new bb::cascades::GroupDataModel(this);
        m_dialogsModel = new DialogDataModel(this);
        m_dialogsModel->setGrouping(ItemGrouping::None);
        m_dialogsModel->setSortedAscending(false);
        keys.clear();
        keys << "date";
//        m_dialogsModel->setSortingKeys(keys);
        Q_ASSERT(m_dialogsModel);

        m_communitiesModel = new DialogDataModel(this);
        m_communitiesModel->setGrouping(ItemGrouping::None);
        m_communitiesModel->setSortedAscending(false);
        Q_ASSERT(m_communitiesModel);


        m_global_chat_container = new QMap<uint, QMap<uint, Vkmessage*>*>();
        Q_ASSERT(m_global_chat_container);
        m_global_music_container = new QMap<uint, VKMusic*>();
        Q_ASSERT(m_global_music_container);
        m_communityCache = new QHash<uint, VKCommunity*>();
        Q_ASSERT(m_communityCache);

//        m_global_messageCache = new QMap<uint, Vkmessage*>();
//        Q_ASSERT(m_global_messageCache);

        m_global_contactsCache = new QMap<uint, Vkcontact*>();
        Q_ASSERT(m_global_contactsCache);

        bool connect_result;

        connect_result = connect(this, SIGNAL(photoDownloaded(int,int,int, QString)),
                                 this, SLOT(attachmentPhotoDownloaded(int,int,int, QString)));
        Q_ASSERT(connect_result);

        manager2 = new QNetworkAccessManager(this);
        connect_result = connect(manager2, SIGNAL(finished(QNetworkReply*)),
                                 this, SLOT(onMediaDownloaded(QNetworkReply*)));
        Q_ASSERT(connect_result);

        Q_UNUSED(connect_result);

        mediaPlayer = new bb::multimedia::MediaPlayer(this);
        Q_ASSERT(mediaPlayer);
        mediaPlayer->prepare();
        mediaPlayer->setVolume(1.0);
        mediaPlayer->setAutoPause(true);
        m_playedTracks = 0;
        c_track_limit  = 5;
        // Restore EQ preset
        if (m_Settings->contains("app/EQ"))
        {
            mediaPlayer->setEqualizerPreset((bb::multimedia::EqualizerPreset::Type)m_Settings->value("app/EQ").toInt());
        }

        np = new NowPlayingConnection("imVKConnection", this);
//        np = new NowPlayingConnection(this);
        Q_ASSERT(np);
        np->setOverlayStyle(bb::multimedia::OverlayStyle::Fancy);

        // Fill attachment type map, as c++ doesn't allow to build switches on strings
        attachmentTypeMap.insert("photo",  _attachment_photo);
        attachmentTypeMap.insert("video",  _attachment_video);
        attachmentTypeMap.insert("audio",  _attachment_audio);
        attachmentTypeMap.insert("doc",    _attachment_doc);
        attachmentTypeMap.insert("wall",   _attachment_wall);
        attachmentTypeMap.insert("sticker",_attachment_sticker);
        attachmentTypeMap.insert("unknown",_attachment_unknown);

        deactivatedReason.insert("banned",  _deact_banned);
        deactivatedReason.insert("deleted", _deact_deleted);
    }

    vk_chat_contoller::~vk_chat_contoller()
    {
        // TODO Auto-generated destructor stub
    }

    QString vk_chat_contoller::getCameraPath() const
    {
        return _cameraPath;
    }

    void vk_chat_contoller::getPaths()
    {
        QString workingDir = QDir::currentPath();
        _cameraPath     = QString("file://" + workingDir + "/shared/photos/imVK/");
        _documentsPath  = QString("file://" + workingDir + "/shared/documents/imVK/");
        _videosPath     = QString("file://" + workingDir + "/shared/videos/imVK/");

        QDir dir; // New QDir objects default to the application working directory.
        bool result = dir.mkpath("data/avatars");
        if(result)
        {
            qDebug() << "Avatars dir created";
        }
        else
        {
            qDebug() << "Avatars dir isn't created. Possibly exists";
        }

        result = dir.mkpath("/" + workingDir + "/shared/photos/imVK/");
        if(result)
        {
            qDebug() << "VK Chat photos dir created";
        }
        else
        {
            qDebug() << "VK Chat photos isn't created. Possibly exists";
        }

        result = dir.mkpath("/" + workingDir + "/shared/documents/imVK/");
        if(result)
        {
            qDebug() << "VK Chat documents dir created";
        }
        else
        {
            qDebug() << "VK Chat documents isn't created. Possibly exists";
        }

        result = dir.mkpath("/" + workingDir + "/shared/videos/imVK/");
        if(result)
        {
            qDebug() << "VK Chat videos dir created";
        }
        else
        {
            qDebug() << "VK Chat videos isn't created. Possibly exists";
        }

        result = dir.mkpath("/" + workingDir + "/shared/music/imVK/");
        if(result)
        {
            qDebug() << "VK Chat music dir created";
        }
        else
        {
            qDebug() << "VK Chat music isn't created. Possibly exists";
        }

        _avatarPath = QString("file://" + workingDir + "/data/avatars/");

        QDir dir2; // New QDir objects default to the application working directory.
        result = dir.mkpath("data/attachments");
        if(result)
        {
            qDebug() << "Attachments dir created";
        }
        else
        {
            qDebug() << "Attachments dir isn't created. Possibly exists";
        }
        //dir.mkpath("data/stickers");
        _attachmentsPath = QString("file://" + workingDir + "/data/attachments/");
    }

    void vk_chat_contoller::setAuthData(QString accessToken, QString userId, int expiriesIn)
    {
        qDebug() << "Setting auth data: Token:" << accessToken << ", userID:" << userId;
        _vkapi = new vk_api(accessToken, userId, expiriesIn, vk_app_id, true);
//        _vkapi = new vk_api(accessToken, userId, expiriesIn, vk_app_id, true);

        // Connect to slots of vk_api
        bool connect_result;
        connect_result = connect(_vkapi, SIGNAL(contactListUpdated(QVariant)),
                this, SLOT(onContactListUpdated(QVariant)));
        Q_ASSERT(connect_result);
        connect_result = connect(_vkapi, SIGNAL(chatUpdated(uint, QVariant)),
                        this, SLOT(chatListUpdated(uint, QVariant)));
        Q_ASSERT(connect_result);
        connect_result = connect(_vkapi, SIGNAL(newMessageReceived(vk_api::new_message_struct, QVariant)),
                this, SLOT(onNewMessageReceived(vk_api::new_message_struct, QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(myInfoUpdated(QVariant)),
                        this, SLOT(myInfoUpdated(QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(chatUpdatedLPS(QVariant)),
                                this, SLOT(chatListUpdatedLPS(QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(musicUpdated(QVariant)),
                                 this, SLOT(onMusicUpdated(QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(dialogsUpdated(QVariant)),
                                 this, SLOT(onDialogsUpdated(QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(groupsUpdated(QVariant)),
                                 this, SLOT(onCommunitiesUpdated(QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(usersInfoUpdated(QVariant)),
                                 this, SLOT(onUsersInfoUpdated(QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(wallDownloaded(QVariant)),
                                         this, SLOT(onWallDownloaded(QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(wallCommentsDownloaded(QVariant)),
                                 this, SLOT(onWallCommentsDownloaded(QVariant)));
        Q_ASSERT(connect_result);

        connect_result = connect(_vkapi, SIGNAL(userOnlineChanged(uint,bool)),
                                 this, SLOT(onUserOnlineChanged(uint,bool)));
        Q_ASSERT(connect_result);
        // Connections for now playing
        connect_result = connect(np, SIGNAL(acquired()),
                                 this, SLOT(onNowPlayingAcquired()));
        Q_ASSERT(connect_result);
        connect_result = connect(np, SIGNAL(revoked()),
                                 this, SLOT(onNowPlayingRevoked()));
        Q_ASSERT(connect_result);
        connect_result = connect(np, SIGNAL(play()),
                                 this, SLOT(onNowPlayingPlay()));
        Q_ASSERT(connect_result);
        connect_result = connect(np, SIGNAL(pause()),
                                 this, SLOT(onNowPlayingPause()));
        Q_ASSERT(connect_result);

        connect_result = connect(mediaPlayer, SIGNAL(mediaStateChanged(bb::multimedia::MediaState::Type)),
                                 np, SLOT(setMediaState(bb::multimedia::MediaState::Type)));
        Q_ASSERT(connect_result);

        connect_result = connect(mediaPlayer, SIGNAL( equalizerPresetChanged (bb::multimedia::EqualizerPreset::Type)),
                                 this, SLOT(onEqChanged(bb::multimedia::EqualizerPreset::Type)));
        Q_ASSERT(connect_result);

        connect_result = connect(mediaPlayer, SIGNAL(metaDataChanged(QVariantMap)),
                                 np, SLOT(setMetaData(QVariantMap)));
        Q_ASSERT(connect_result);

        connect_result = connect(np, SIGNAL(next()),
                                 mediaPlayer, SLOT(nextTrack()));
        Q_ASSERT(connect_result);

        connect_result = connect(np, SIGNAL(previous()),
                                 mediaPlayer, SLOT(previousTrack()));
        Q_ASSERT(connect_result);

//        connect_result = connect(this, SIGNAL(latestMessageUpdated(Vkmessage*)),
//                                 this, SLOT(onLatestMessageUpdated(Vkmessage*)));
//        Q_ASSERT(connect_result);


        Q_UNUSED(connect_result);
        m_authorized = true;


        storeAccountSettings(accessToken, userId);

        // Tell the UI that we got the token and show is starting
        emit authTokenReceived();
        updateContactsModel();
    }

    void vk_chat_contoller::storeAccountSettings(QString &accessToken, QString &userId)
    {
        // Store data to be accessible for headless service
        if (!m_Settings) {
            m_Settings = new QSettings("Kartun", "imVKService", this);
        }
//            _accountIdKey("hub/accountId"),
//            _accountNameKey("hub/accountName"),
//            _categoryIdKey("hub/categoryId"),
//            _categoriesKey("hub/categories"),
//            _categoryNameKey("hub/categoryName"),
//            _lastItemIdKey("hub/lastItemId"),
//            _itemsKey("hub/items"),
//            m_Settings->setValue("hub/accountId", userId);

            m_Settings->setValue("vk_api/accountToken", accessToken);
            m_Settings->setValue("vk_api/userID", userId);
            m_Settings->setValue("vk_api/applicationID",vk_app_id);
            m_Settings->setValue("hub/accountName", "imVK");
            m_Settings->setValue("hub/categoryName", "messages");
            m_Settings->sync();
            qDebug() << "Stored settings for HUB";
//            QStringList
//        }
    }

    bb::cascades::GroupDataModel* vk_chat_contoller::contactsModel() const
    //bb::cascades::ArrayDataModel* vk_chat_contoller::contactsModel() const
    {
        updateContactsModel();
        return m_contactsModel;
    }

    bb::cascades::GroupDataModel* vk_chat_contoller::chatModel() const
    {
        // If no chat have been downloaded yet, download initial part of the chat
        if (m_chatModel2->isEmpty())
        {
            updateChatModel();
        }

        return m_chatModel;
//        qDebug() << m_chatModel->toListOfMaps();

    }

    bb::cascades::GroupDataModel* vk_chat_contoller::getDialogsModel() const
    {
        if (m_dialogsModel->isEmpty())
        {
            updateDialogsModel();
        }
        return m_dialogsModel;
    }

    bb::cascades::GroupDataModel* vk_chat_contoller::getCommunitiesModel() const
    {
        if (m_communitiesModel->isEmpty())
        {
            updateCommunities();
        }
        return m_communitiesModel;
    }

    bb::cascades::GroupDataModel* vk_chat_contoller::getWallModel() const
    {
        return m_wallModel;
    }

    bb::cascades::GroupDataModel* vk_chat_contoller::getWallCommentsModel() const
    {
        return m_wallCommentsModel;
    }

    void vk_chat_contoller::fetchWallComments(int wallID, uint post_id, bool isCommunity)
    {
        m_wallCommentsModel->clear();
        _vkapi->vk_get_wall_comments(_currentCommunity, post_id, isCommunity);
    }

    void vk_chat_contoller::updateContactsModel() const
    {
        m_contactsModel->clear();
        if (m_authorized)
        {
            // Add the data to the model
            _vkapi->vk_get_all_friends();
        }
    }

    void vk_chat_contoller::updateMusicModel() const
    {
        m_musicModel->clear();
        if (m_authorized)
        {
            // Add the data to the model
            _vkapi->vk_get_my_music();
        }
    }

    void vk_chat_contoller::searchMusic(QString searchTerm)
    {
        m_musicModel->clear();
        if (m_authorized)
        {
            // Add the data to the model
            _vkapi->vk_search_music(searchTerm);
        }
    }

    void vk_chat_contoller::getPopularMusic()
    {
        m_musicModel->clear();
        if (m_authorized)
        {
            _vkapi->vk_get_popular_music();
        }
    }

    void vk_chat_contoller::getRecommendedMusic(uint trackID, uint ownerID)
    {
        m_musicModel->clear();
        if (m_authorized)
        {
            _vkapi->vk_get_recommended_music(trackID, ownerID);
        }
    }

    void vk_chat_contoller::updateDialogsModel() const
    {
        m_dialogsModel->clear();
        qDebug() << "Updating dialogs: Authorized:" << m_authorized;
        if (m_authorized)
        {
            _vkapi->vk_get_dialogs();
        }
    }

    void vk_chat_contoller::updateCommunities() const
    {
        m_communitiesModel->clear();
        if (m_authorized)
        {
            _vkapi->vk_get_groups();
        }
    }

    void vk_chat_contoller::updateChatModel() const
    {
        // TODO :: Confirm and remove this method
        //m_chatModel->clear();

        // TODO :: Make proper assert
        //Q_ASSERT(testList[1].toMap().value("from_id"));
        qDebug() << "Requesting chat with:" << _currentContactID;
        if (m_authorized)
        {
            // Add the data to the model
            _vkapi->vk_get_messages_with_user(_currentContactID);
        }
        else
        {
            qDebug() << "Unauthorized. Can't load chat";
        }
    }


    void vk_chat_contoller::onContactListUpdated(const QVariant &list)
    {
        //QVariantList _list = list.value<QVariantList>();
//        qDebug() << list;
//        qDebug() << list.value<QVariantMap>();
       // qDebug() << list.value<QVariantMap>().value("response").value<QVariantList>();
       // qDebug() << "QVarianList" << list.value<QVariantList>();
        //qDebug() << list.value<QVariantMap>().value("response").value();

        // TODO :: Confirm re-inteble
        //_contactsData.clear();
        QVariantList dataList = list.value<QVariantList>();
        vk_api::my_info contact_info;
        foreach (const QVariant &v, dataList)
        {
            QVariantMap map = v.value<QVariantMap>();
            Vkcontact* newContact = new Vkcontact(map.value("user_id").toUInt(), map.value("first_name").toString(),
                                                  map.value("last_name").toString(), "", map.value("online").toBool() );


            if (map.count("deactivated") > 0)
            {
                //QString _reason = map.value("deactivated").toString();
                int _reason = deactivatedReason.value(map.value("deactivated").toString());
                newContact->setDeactivated(_reason);
            }
//            contact_info.first_name = map.value("first_name").toString();
//            contact_info.last_name  = map.value("last_name").toString();
//            contact_info.userID     = map.value("user_id").toString();

            // TODO :: Check
            m_contactsModel->insert(newContact);
            m_global_contactsCache->insert(newContact->getContactID(), newContact);

            // Avatar downloading and overriding
            bool localAvaiable = downloadAvatar(map.value("photo_100").toString(), Contact, newContact->getContactID());
            if (localAvaiable)
            {
                QFileInfo fileInf(map.value("photo_100").toString());
                QString fileName = fileInf.fileName();
                contact_info.localAvatar   = _avatarPath + fileName;
                //map.insert("photo_100", contact_info.localAvatar);

                QVariantList match = m_contactsModel->findExact(newContact);
                newContact->setSmallAvatar(_avatarPath + fileName);
                bool result = m_contactsModel->updateItem(match, newContact);
                Q_ASSERT(result);
                Q_UNUSED(result);

//                qDebug() << map;
            }
        }
    }

    void vk_chat_contoller::onUsersInfoUpdated(const QVariant& list)
    {
        QVariantList dataList = list.value<QVariantList>();
//        bool usersUpdated = false;
        foreach (const QVariant &v, dataList)
        {
            Vkcontact* newContact = new Vkcontact(v, this);
            uint l_contactID = newContact->getContactID();
            if (!m_global_contactsCache->value(l_contactID))
            {
                m_global_contactsCache->insert(l_contactID, newContact);
//                usersUpdated = true;
                bool localAvaiable = downloadAvatar(newContact->getSmallAvatar(), Contact, l_contactID);
                if (localAvaiable)
                {
                    QFileInfo fileInf(newContact->getSmallAvatar());
                    QString fileName = fileInf.fileName();

                    // This received from dialogs, so don't show in actual contact list
                    newContact->setSmallAvatar(_avatarPath + fileName);
                }
                m_dialogsModel->updateByUserID(l_contactID);
            }
        }
        // TODO :: Finish. It doens't update UI
//        if (usersUpdated)
//            emit dialogsUpdated(m_dialogsModel);
    }

    void vk_chat_contoller::onCommunitiesUpdated(const QVariant& list)
    {
        QVariantList dataList = list.value<QVariantMap>().value("items").value<QVariantList>();
        qDebug() << "Communities list:" << dataList;
        //        bool usersUpdated = false;
        foreach (const QVariant &v, dataList)
        {
            VKCommunity* newCommunity = new VKCommunity(v, this);
            uint l_commID = newCommunity->getId();
//            if (!m_global_contactsCache->value(l_contactID))
//            {
                m_communityCache->insert(l_commID, newCommunity);
//                usersUpdated = true;
                qDebug() << "Checking community avatar:" << newCommunity->getPhoto200();
                bool localAvaiable = downloadAvatar(newCommunity->getPhoto200(), Community, l_commID);
                if (localAvaiable)
                {
                    QFileInfo fileInf(newCommunity->getPhoto200());
                    QString fileName = fileInf.fileName();

                    qDebug() << "Community avatar locally available:" << _avatarPath + fileName;
                    // This received from dialogs, so don't show in actual contact list
                    newCommunity->setPhoto200(_avatarPath + fileName);
                }
                m_communitiesModel->insert(newCommunity);
//                m_dialogsModel->updateByUserID(l_contactID);
//            }
        }
    }

    void vk_chat_contoller::onWallDownloaded(const QVariant &list)
    {
//        QVariantList dataList = list.value<QVariantMap>("items");
        qDebug() << "Parsing wall:" << list;
        m_wallModel->clear();
        QVariantList dataList = list.value<QVariantMap>().value("items").value<QVariantList>();
        foreach (const QVariant &v, dataList)
        {
            VKWall* newWall = new VKWall(v, this);
//            uint l_commID = newWall->getId();
//            if (!m_global_contactsCache->value(l_contactID))
//            {
//                m_communityCache->insert(l_commID, newCommunity);
//                usersUpdated = true;
//                qDebug() << "Checking community avatar:" << newCommunity->getPhoto200();
//                bool localAvaiable = downloadAvatar(newCommunity->getPhoto200(), Community, l_commID);
//                if (localAvaiable)
//                {
//                    QFileInfo fileInf(newCommunity->getPhoto200());
//                    QString fileName = fileInf.fileName();
//
//                    // This received from dialogs, so don't show in actual contact list
//                    newCommunity->setPhoto200(_avatarPath + fileName);
//                }
                m_wallModel->insert(newWall);
//                m_dialogsModel->updateByUserID(l_contactID);
//            }
        }
    }

    void vk_chat_contoller::onWallCommentsDownloaded(const QVariant& list)
    {
        qDebug() << "Parsing wall:" << list;
        m_wallModel->clear();
        QVariantList dataList = list.value<QVariantMap>().value("items").value<QVariantList>();
        foreach (const QVariant &v, dataList)
        {
            VKWallComment* newWallComment = new VKWallComment(v, this);
            m_wallCommentsModel->insert(newWallComment);
        }
    }

    void vk_chat_contoller::onUserOnlineChanged(const uint& userID, const bool& online)
    {
        qDebug() << "Online changed:" << userID << "online" << online;
        if (m_global_contactsCache->value(userID))
        {
            Vkcontact* l_contact = m_global_contactsCache->value(userID);
            l_contact->setOnline(online);
            // Update status in dialogs pane
            m_dialogsModel->updateByUserID(userID);
        }
    }

    void vk_chat_contoller::myInfoUpdated(const QVariant &list)
    {
        QVariantList dataList = list.value<QVariantList>();
        foreach (const QVariant &v, dataList)
        {
            QVariantMap map = v.value<QVariantMap>();

//            Vkcontact* m_myInfo = new Vkcontact(map.value("id").toUInt(), map.value("first_name").toString(), map.value("last_name").toString());
            m_myInfo = new Vkcontact(v, this);
            m_global_contactsCache->insert(m_myInfo->getContactID(), m_myInfo);
            qDebug() << "Adding me with ID" << m_myInfo->getContactID() << " to global cache";

            // Avatar downloading and overriding
            bool localAvaiable = downloadAvatar(map.value("photo_100").toString(), Contact, m_myInfo->getContactID());
            if (localAvaiable)
            {
                QFileInfo fileInf(map.value("photo_100").toString());
                QString fileName = fileInf.fileName();
                m_myInfo->setSmallAvatar(_avatarPath + fileName);
                qDebug() << map;
            }
            emit myInfoUpdated(m_myInfo);
        }
    }

    void vk_chat_contoller::chatListUpdated(uint fromID,const  QVariant &list)
    {
//        qDebug() << list.value<QVariantMap>().value("response").value<QVariantList>();
        //QVariantList testList = list.value<QVariantMap>().value("response").value<QVariantList>();
//        qDebug() << list.value<QVariantMap>().value("response").value<QVariantMap>().value("items").value<QVariantList>();
        QVariantList testList = list.value<QVariantMap>().value("items").value<QVariantList>();
        QVariantMap entry;
        bool messageInserted = false;

        // Determine, what chat we downloaded
//        bb::cascades::GroupDataModel *t_chatModel;  // Temporary data model

        // TODO :: Make proper assert
        //Q_ASSERT(testList[1].toMap().value("from_id"));

        // TODO :: Add from_id to signature, would be much simplier
        // uint user_id = testList[0].toMap().value("user_id").toUInt();
        if (!this->m_global_chat_container->value(fromID) )
        {
            // No history yet.  Need to create new container
            QMap<uint, Vkmessage*>* _newCache = new QMap<uint, Vkmessage*>();
            // TODO :: Refactor
            this->m_global_chat_container->insert(fromID, _newCache);
        }

        // Seems this shit is bugged
        QListIterator<QVariant> i(testList);
        i.toBack();
//        foreach(const QVariant &v, testList )
        while (i.hasPrevious())
        {
            const QVariant &v = i.previous();
            if (!v.toMap().value("body").isNull())
            {
                qDebug() << "Message body:" <<v.toMap().value("body");

                QMap<QString, QVariant> responseMap = v.toMap();
//
//                uint _fromID    = responseMap.value("from_id").toUInt();
//                uint _userID    = responseMap.value("user_id").toUInt();
////                uint _timestamp = responseMap.value("date").toUInt();
//                int  _messageID = responseMap.value("id").toInt();
//                bool _out       = responseMap.value("out").toBool();
//
//                // TODO :: implement simple unescape
//                QList<Vkattach*>  _attachments;
//                QString _url;

                // check for attachments
//                if (!responseMap.value("attachments").isNull())
//                {
////                    qDebug() << responseMap.value("attachments");
//                    QVariantList attachmentsList = responseMap.value("attachments").value<QVariantList>();
//                    QString photoURL, photoURL_large, _url;
//                    int _attachmentType;
//                    foreach (const QVariant v2, attachmentsList)
//                    {
//                        _attachments.append(new Vkattach(_messageID, _out, v2, this) );
//                    }
//                    emit message_parsed(responseMap.value("id").toString());
//                }
                //entry["photo_100"] = "none";
                //m_chatModel->insert(entry);

//                if (_out) { _userID = _fromID; }
//                Vkmessage* newMessage2 = new Vkmessage(_messageID, _fromID,_userID, _timestamp, _out,
//                                                         (Vkmessage::readState)responseMap.value("read_state").toInt(),
//                                                         "", responseMap.value("body").toString() + _url, 1 , this);
                Vkmessage* newMessage2 = new Vkmessage(v, m_myInfo->getContactID(), this);
//                newMessage2->addAttachments(_attachments);
                // Don't show own messages on Active Frame
                if (!newMessage2->getOut()) {m_latestMessage = newMessage2;}
                qDebug() << "FromID:" << newMessage2->getFromID();
//                t_chatModel->insert(newMessage2);
                //newMessage2->appendText(" \r\ntest");
                // Other person ID is the key
                m_global_chat_container->value(fromID)->insert(newMessage2->getMessageID(),newMessage2);
                messageInserted = true;
                //m_global_messageCache->insert(newMessage2->getMessageID(), newMessage2);
                if (fromID == _currentContactID)
                {
//                    for (int i=0; i<_attachments.count(); ++i)
//                    {
//                        // Insert attachments
//                        m_chatModel->insert(_attachments.value(i));
//                    }
//                    // This stupid junk insert in front without respecting keys
//                    m_chatModel->insert(newMessage2);
                    m_chatModel2->insertItem(newMessage2);
                    emit chatUpdated(m_chatModel2);
                }
            }
        }
        if (messageInserted)
        {
            emit latestMessageUpdated(m_latestMessage);
        }

//        if (fromID == _currentContactID)
//        {
//            const QList<kartun_vkchat::Vkmessage*> tmp = m_global_chat_container->value(fromID)->values();
//            m_chatModel->insertList(tmp);
//        }

        //m_chatModel->insertList( list.value<QVariantMap>().value("response").value<QVariantList>());
    }

    void vk_chat_contoller::chatListUpdatedLPS(const QVariant &list)
    {
        //        qDebug() << list.value<QVariantMap>().value("response").value<QVariantList>();
                //QVariantList testList = list.value<QVariantMap>().value("response").value<QVariantList>();
        //        qDebug() << list.value<QVariantMap>().value("response").value<QVariantMap>().value("items").value<QVariantList>();
                QVariantList testList = list.value<QVariantMap>().value("messages").value<QVariantMap>().value("items").value<QVariantList>();
                qDebug() << "Parsing message list from LPS";
                QVariantMap entry;
                bool messageInserted = false;
                uint prevLatestMessageTimestamp = m_latestMessage->getTimestamp();

                // Seems this shit is bugged
                QListIterator<QVariant> i(testList);
                i.toBack();

                while (i.hasPrevious())
                {
                    const QVariant &v = i.previous();
                    if (!v.toMap().value("body").isNull())
                    {
                        qDebug() << "Message body:" << v.toMap().value("body");

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

                        if (!this->m_global_chat_container->value(l_fromID) )
                        {
                            // No history yet.  Need to create new container
                            QMap<uint, Vkmessage*>* _newCache = new QMap<uint, Vkmessage*>();
                            // TODO :: Refactor
                            this->m_global_chat_container->insert(l_fromID, _newCache);
                        }
                        else
                        {
                            // Skip duplicates
                            Vkmessage *local = m_global_chat_container->value(l_fromID)->value(_messageID);
                            qDebug() << "Skip duplicate local message:" << local;
                            if (local)
                            {
                                continue;
                            }
                        }
                        Vkmessage* newMessage2 = new Vkmessage(v, m_myInfo->getContactID(), this);

                        // Don't show own messages on Active Frame
                        if (!newMessage2->getOut()) {
                            // Server sends only user_id ...
                            m_latestMessage = newMessage2;
                        }

                        qDebug() << "FromID:" << newMessage2->getUserID();
                        // Other person ID is the key
                        m_global_chat_container->value(l_fromID)->insert(newMessage2->getMessageID(),newMessage2);
                        messageInserted = true;
                        qDebug() << "From id:" << l_fromID << ", current:" << _currentContactID;
                        if (l_fromID == _currentContactID)
                        {
                            m_chatModel2->insertItem(newMessage2);
                            emit chatUpdated(m_chatModel2);
                        }
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
                    emit chatUpdated(m_chatModel2);
                    emit latestMessageUpdated(m_latestMessage);
                }
//                onLatestMessageUpdated(m_latestMessage);
                if (prevLatestMessageTimestamp < m_latestMessage->getTimestamp() &&
                    m_latestMessage->getOut() == false)
                    onLatestMessageUpdated(m_latestMessage);
    }

    void vk_chat_contoller::onLatestMessageUpdated(Vkmessage* latestMessage)
    {
        Q_UNUSED(latestMessage);
        // display notification
        Notification* pNotification = new Notification();

        Vkcontact* contact = m_global_contactsCache->value(m_latestMessage->getFromID());
        if (contact) {
            pNotification->setTitle(QString("%1 %2").arg(contact->getFirstName(),contact->getLastName()));
        } else {pNotification->setTitle(tr("New message")); _vkapi->vk_get_users_info(QStringList() << QString::number(m_latestMessage->getFromID())); }
        pNotification->setBody(m_latestMessage->getText());
        qint64 l_actualTimestamp = (quint64)(m_latestMessage->getTimestamp()) * 1000;
        pNotification->setTimestamp(l_actualTimestamp);
        qDebug() << "Sending notification:" << pNotification->title() << " @ " << pNotification->timestamp();
        if (contact)
        {
            pNotification->setIconUrl(QUrl(contact->getSmallAvatar()));
            qDebug() << "Settings avatar for notification:" << pNotification->iconUrl();
        }
        else
        {
            pNotification->setIconUrl(QUrl(QString("file://%1/app/native/assets/img/icon_110.png").arg(QDir::currentPath())));
        }

        NotificationError::Type result = pNotification->notify();
        qDebug() << "Notification result" << result;
    }

    void vk_chat_contoller::setCurrentContactID(const uint contactID)
    {
        // We jumped to dialog
        qDebug() << "vk_chat_contoller::setCurrentContactID - " << contactID;
        _currentContactID = contactID;
        // Here we need to check, if we already have container for the chat. If it's not
        // created yet, need to create one.
        if (this->m_global_chat_container->value(_currentContactID) )
        {
//                t_chatModel = m_global_chat_container->value(_currentContactID);
        }
        else
        {
            // TODO :: Refactor, it repeats 4 times !!!
            QMap<uint, Vkmessage*>* _newCache = new QMap<uint, Vkmessage*>();
            this->m_global_chat_container->insert(_currentContactID, _newCache);
        }
        m_currentContact = m_global_contactsCache->value(contactID);
    }

    void vk_chat_contoller::sendMessageToCurrentContact(QString messageText)
    {
        QString test = messageText + ". Sent from VK messenger for Blackberry 10";
        //_vkapi->vk_send_message(_currentContactID, test);
        _vkapi->vk_send_message(_currentContactID, messageText);
    }

    void vk_chat_contoller::unsetCurrentContact()
    {
        m_currentContact = NULL;
        _currentContactID = 0;
        m_chatModel2->clear();
    }

    void vk_chat_contoller::sortContacts(uint sortType)
    {
        QStringList keys;
        switch (sortType) {
            case 0: keys << "last_name" << "first_name"; break;
            case 1: keys << "online"; break;
            case 2: keys << "deactivated"; break;
            default: Q_ASSERT_X(sortType > 2, "vk_chat_contoller::sortContacts", "Invalid sort type");
                break;
        }

//        if (sortType == 1)
//        {
//            this->m_contactsModel->setSortedAscending(false);
//        }
//        else
//        {
//            this->m_contactsModel->setSortedAscending(true);
//        }
        this->m_contactsModel->setSortingKeys(keys);
    }

    void vk_chat_contoller::onNewMessageReceived(vk_api::new_message_struct newMessage, const QVariant &attachments)
    {
//        bb::cascades::GroupDataModel* t_chatModel;  // Temporary data model
        // We could come here, if no chat have been downloaded, but some contact sent us a message
        // Depending on direction, put in proper cache
        // TODO :: Confirm
        uint prevLatestMessageTimestamp = m_latestMessage->getTimestamp();
        uint l_user_id = newMessage.from_id;
        if (newMessage.out) { l_user_id = newMessage.user_id; }
//        if (!this->m_global_chat_container->value(newMessage.from_id) )
        if (!this->m_global_chat_container->value(l_user_id) )
        {

            // No history yet.  Need to create new container
            // TODO :: Refactor
            QMap<uint, Vkmessage*>* _newCache = new QMap<uint, Vkmessage*>();
            this->m_global_chat_container->insert(l_user_id, _newCache);
        }

        // TODO :: Add notification if app is minimized !!
        // TODO :: Test
        //t_chatModel->insert(entry);

        Vkmessage* newMessage2 = new Vkmessage(newMessage, m_myInfo->getContactID(), attachments, this);
        if (!newMessage2->getOut()) { m_latestMessage = newMessage2; }
//        uint _fromID = newMessage2->getFromID();
        uint _fromID = l_user_id;

        // Store in global cache. If value is there, it would be overwritten, otherwise added
        m_global_chat_container->value(_fromID)->insert(newMessage2->getMessageID(),newMessage2);

//        qDebug() << _fromID << " userid:" << newMessage2->getUserID() << "fromid:" << newMessage2->getFromID();
        // Update in dialogs as well

        m_dialogsModel->replaceMessage(_fromID, newMessage2);


        // If this message relevant for the currently displayed chat, update data model
        if (_fromID == _currentContactID)
        {
            QVariantList match = m_chatModel->findExact(newMessage2);

            // TODO :: Check with my datamodel
            m_chatModel2->insertItem(newMessage2);

            // In global contacts cache there is own info, that isn't required in contact list
            if (!match.isEmpty())
            {
                bool result = m_chatModel->updateItem(match, newMessage2);
                Q_ASSERT(result);
                Q_UNUSED(result);
            }
            else
            {
                m_chatModel->insert(newMessage2);
            }
            // This notifies view that new items arrived
            emit chatUpdated(m_chatModel2);
        }

//        t_chatModel->insert(newMessage2);
//        m_global_messageCache->insert(newMessage2->getMessageID(), newMessage2);

//        emit chatUpdated2(t_chatModel);
        emit message_parsed(QString::number(newMessage.message_id));
        emit latestMessageUpdated(m_latestMessage);

        if (prevLatestMessageTimestamp < m_latestMessage->getTimestamp() &&
            m_latestMessage->getOut() == false)
            onLatestMessageUpdated(m_latestMessage);
    }

    bool vk_chat_contoller::downloadAvatar(QString url, ContactType contactType, uint userID)
    {
        // Check, if we already have an avatar
        QFileInfo fileInf(url);
        QString fileName = fileInf.fileName();
        QString localFName = _avatarPath.mid(7);
        QFile   _avatarFile(QString(localFName +fileName));
        if ( _avatarFile.exists() )
        {
            avatarLocallyAvailable(userID, contactType, _avatarFile.fileName());
            return true;
        }
        else
        {
            QNetworkReply* reply = manager2->get(QNetworkRequest(url));
            reply->setProperty("userID", userID);
            reply->setProperty("requestType", _request_avatar);
            reply->setProperty("contactType",contactType);
        }
        return false;
    }

    void vk_chat_contoller::processAvatarDownloaded(QNetworkReply *reply)
    {
        if ( reply->error() == QNetworkReply::NoError )
        {
            uint userID = reply->property("userID").toUInt();
            ContactType contactType = (ContactType)reply->property("contactType").toInt();
            QByteArray buffer = reply->readAll();
            // Get filename from network reply
            QUrl url = reply->url();
            QFileInfo fileInf(url.toString());
            QString fileName = fileInf.fileName();
            QString localFName = _avatarPath.mid(7);
            QFile   _avatarFile(QString(localFName +fileName));
            if ( !_avatarFile.exists() )
            {
                bool result = _avatarFile.open(QIODevice::WriteOnly);
                Q_ASSERT(result);
                Q_UNUSED(result);
                if (_avatarFile.error() != QFile::NoError)
                    qDebug() << _avatarFile.error() << _avatarFile.errorString();
                Q_ASSERT(result);
                _avatarFile.write(buffer);
                _avatarFile.close();
            }
            avatarLocallyAvailable(userID, contactType, _avatarPath+fileName);
        }
        reply->deleteLater();
    }

    void vk_chat_contoller::downloadMedia(QString url, int fromID, int mediaType, int messageID)
    {
        QNetworkReply* reply = manager2->get(QNetworkRequest(url));
        reply->setProperty("fromID", fromID);
        reply->setProperty("mediaType", mediaType);
        reply->setProperty("messageID", messageID);
        reply->setProperty("requestType", _request_media);
    }

    void vk_chat_contoller::processMediaDownloaded(QNetworkReply *reply)
    {
        int mediaType = reply->property("mediaType").toInt();
        int messageID = reply->property("messageID").toInt();
        emit message_parsed(reply->property("messageID").toString());
        int fromID = reply->property("fromID").toInt();
        QString localFName;
        switch (mediaType) {
            case _media_photo:
                  localFName = _cameraPath.mid(7);
                break;
            case _media_doc:
                  localFName = _documentsPath.mid(7);
                break;
            case _media_video:
                  localFName = _videosPath.mid(7);
                break;
            case _media_audio:
                  localFName = _audioPath.mid(7);
                break;
            default:
                break;
        }

       if ( reply->error() == QNetworkReply::NoError )
       {
           QByteArray buffer = reply->readAll();
           // Get filename from network reply
           QUrl url = reply->url();
           QFileInfo fileInf(url.toString());
           QString fileName = fileInf.fileName();

           QFile   _mediaFile(QString(localFName +fileName));
           {
               bool result;
               result = _mediaFile.open(QIODevice::WriteOnly);
               qDebug() << _mediaFile.error() << _mediaFile.errorString();
               Q_ASSERT(result);
               Q_UNUSED(result);
               _mediaFile.write(buffer);
               //out << "This is a text file\n";
               _mediaFile.close();
               switch (mediaType) {
                   case _media_photo:
                       emit photoDownloaded(fromID, mediaType, messageID, _mediaFile.fileName());
                       break;
                   case _media_doc:
                         localFName = _documentsPath.mid(7);
                       break;
                   case _media_video:
                         localFName = _videosPath.mid(7);
                       break;
                   case _media_audio:
                         localFName = _audioPath.mid(7);
                       break;
                   default:
                       break;
               }
           }
       }
       else
       {
           qDebug() << reply->errorString();
       }
       reply->deleteLater();
    }

    void vk_chat_contoller::attachmentPhotoDownloaded(int fromID, int mediaType, int messageID, QString filename)
    {
//        bb::cascades::GroupDataModel* t_chatModel;  // Temporary data model
        // We could come here, if no chat have been downloaded, but some contact sent us a message
//        if (this->m_global_chat_container->value(fromID) )
//        {
////            t_chatModel = m_global_chat_container->value(fromID);
//        }
//        else
//        {
//            // No history yet.  Need to create new container
////            t_chatModel = new bb::cascades::GroupDataModel();
////            Q_ASSERT(t_chatModel);
////            QStringList keys2;
////            keys2 << "date" << "id";
////            t_chatModel->setSortingKeys(keys2);
//            // TODO :: Refactor
//            QMap<uint, Vkmessage*>* _newCache = new QMap<uint, Vkmessage*>();
//            this->m_global_chat_container->insert(fromID, _newCache);
//        }
//
//        vk_api::new_message_struct newMessage;
//        newMessage.from_id = fromID;
//        // TODO :: Make proper
//        newMessage.message_id = messageID + 1;
//        newMessage.text = "file://" + filename;
//        newMessage.messageContentType = mediaType;
//        // TODO :: Make proper
//        newMessage.timestamp = QDateTime::currentMSecsSinceEpoch();
//        //if (fromID == _vkapi->m)
//        newMessage.out  = false;
//        onNewMessageReceived(newMessage);

//        entry["body"] = v.toMap().value("body").toString();
//        entry["id"]  = v.toMap().value("id");
//        entry["user_id"] = v.toMap().value("user_id");
//        entry["from_id"] = v.toMap().value("from_id");
//        entry["read_state"] = v.toMap().value("read_state");
//        entry["date"] = v.toMap().value("date");
//        entry["out"] = v.toMap().value("out");

//        QVariantList _message = t_chatModel->find(QVariantList(messageID));
//
//        entry["body"]       = newMessage.text;
//        entry["id"]         = qlonglong(newMessage.message_id);
//        entry["user_id"]    = qlonglong(newMessage.user_id);
//        entry["from_id"]    = qlonglong(newMessage.user_id);
//        entry["read_state"] = qlonglong(newMessage.read);
//        entry["date"]       = qlonglong(newMessage.timestamp);
//        entry["out"]        = qlonglong(newMessage.out);
//        qDebug() << QString::number(newMessage.user_id);
//        vk_api::my_info abc = _contactsData.value(QString::number(newMessage.user_id));
//        entry["first_name"] = abc.first_name;
//        entry["last_name"]  = abc.last_name;
//        entry["photo_100"]  = abc.localAvatar;
//        qDebug() << _contactsData.value(QString(newMessage.user_id)).localAvatar;
//        //entry["photo_100"] = "none";
//        // This  might screw thing up, if we in one chat, and receive message from another
//
//        // TODO :: Add notification if app is minimized !!
//        t_chatModel->insert(entry);
//        emit chatUpdated2(t_chatModel);
    }

    void vk_chat_contoller::onMediaDownloaded(QNetworkReply* reply)
    {
        QNetworkReply::NetworkError _errCode = reply->error();

        if ( (_errCode != QNetworkReply::NoError) && ( _errCode != QNetworkReply::UnknownContentError) )
        {
            QString networkError = reply->errorString();
            emit networkErrorOccured(_errCode, networkError);
            return; // ??
        }
        int requestType = reply->property("requestType").toInt();
        switch (requestType) {
            case _request_avatar:
                    processAvatarDownloaded(reply);
                    break;
            case _request_media:
                    processMediaDownloaded(reply);
        }
    }

    void vk_chat_contoller::avatarLocallyAvailable(uint userID, ContactType contactType, QString localFileName)
    {
        // TODO :: Make generic
        QVariantList match;
        Vkcontact* currContact;
        switch (contactType)
        {
            case Contact :
                currContact = m_global_contactsCache->value(userID);
                Q_ASSERT(currContact);
                match = m_contactsModel->findExact(currContact);
                currContact->setSmallAvatar(localFileName);

                // In global contacts cache there is own info, that isn't required in contact list
                if (!match.isEmpty())
                {
                    bool result = m_contactsModel->updateItem(match, currContact);
                    Q_ASSERT(result);
                    Q_UNUSED(result);
                }
                break;
            case Community:
                VKCommunity* currCommunity = m_communityCache->value(userID);
                Q_ASSERT(currCommunity);
                match = m_communitiesModel->findExact(currCommunity);
                currCommunity->setPhoto200(localFileName);

                // In global contacts cache there is own info, that isn't required in contact list
                if (!match.isEmpty())
                {
                    bool result = m_communitiesModel->updateItem(match, currCommunity);
                    Q_ASSERT(result);
                    Q_UNUSED(result);
                }
                break;
        }
    }

    Vkcontact* vk_chat_contoller::getCurrentContact() const
    {
        return m_currentContact;
    }

    VKCommunity* vk_chat_contoller::getCurrentCommunity() const
    {
        qDebug() << "Returning:" << m_currentCommunity->getId() << m_currentCommunity->getName();
        return m_currentCommunity;
    }

    Vkmessage* vk_chat_contoller::getLatestMessage() const
    {
//        qDebug() << m_latestMessage->getText();
        return m_latestMessage;
    }

    QString vk_chat_contoller::getContactFullName(uint userID)
    {
        Vkcontact* _contact = m_global_contactsCache->value(userID);
        if (!_contact) { _contact = new Vkcontact(this); }
        return QString("%1 %2").arg(_contact->getFirstName(), _contact->getLastName());
    }

    Vkcontact* vk_chat_contoller::getContact(uint userID)
    {
        Vkcontact* _tmp = m_global_contactsCache->value(userID);
        if(!_tmp)
        {
           _tmp = new Vkcontact(this);
        }
        return _tmp;
    }

//    bool vk_chat_contoller::isVirtualKeyboardVisible()
//    {
//        return virtualkeyboard_
//    }

    kartun_vkchat::MessageDataModel* vk_chat_contoller::getChatModel2() const
    {
        qDebug() << "Chat model requested";
        if (m_chatModel2->isEmpty())
        {
            updateChatModel();
        }
        return this->m_chatModel2;
    }

    void vk_chat_contoller::play(uint trackID)
    {
        if (m_playedTracks < c_track_limit)
        {
            VKMusic* track = m_global_music_container->value(trackID);
            if (track != NULL)
            {
                np->setNextEnabled(true);
                np->setPreviousEnabled(true);
                // Build playlist
                uint l_count = 0;
                QMapIterator<uint, VKMusic*> i(*m_global_music_container);
                i.findNext(track);
                QStringList playlist;
                while (i.hasNext())
                {
                    const VKMusic* track2 = i.value();
                    playlist.append(QString("#EXTINF:%1,%2 - %3").arg(QString::number(track2->getDuration()), track2->getArtist().toUtf8(), track2->getTitle().toUtf8()));
                    playlist.append(track2->getUrl());
                    i.next();
                    l_count++;
                    if (l_count >= c_track_limit) { break; }
                }

                QString true_playlist = playlist.join("\n");
                qDebug() << "Playlist items:" << playlist.count();
                // Store temporary playlist
                QString tmpDir = QDir::tempPath();
                QString _playlistPath     = QString(tmpDir + QString("/imVK_%1.m3u").arg(QString::number(m_playlistCounter)));
                QFile   _playlistFile(_playlistPath);
//                if ( !_avatarFile.exists() )
                {
                    bool result = _playlistFile.open(QIODevice::WriteOnly);
                    Q_ASSERT(result);
                    Q_UNUSED(result);
                    if (_playlistFile.error() != QFile::NoError)
                        qDebug() << _playlistFile.error() << _playlistFile.errorString();
                    Q_ASSERT(result);
                    _playlistFile.write(true_playlist.toUtf8());
                    _playlistFile.close();
                    m_playlistCounter++;
                }

                qDebug() << "Setting playlist file:" << "file://"+_playlistPath;
                mediaPlayer->reset();
                mediaPlayer->setSourceUrl("file://"+_playlistPath);
//                mediaPlayer->setSourceUrl(track->getUrl());
//                MediaPlayer->
//                mediaPlayer->set

                if ( mediaPlayer->prepare() == MediaError::None)
                {
                    mediaPlayer->stop();
                    // TODO :: Restore
                    m_playedTracks++;
                    m_songTitle = QString("%1 - %2").arg(track->getArtist(), track->getTitle());
                    emit songTitleChanged(m_songTitle);
                    // Integrate to now playing
                    if (!np->isAcquired())
                    {
                        np->acquire();
                    }
                    else
                    {
                        onNowPlayingAcquired();
                    }

                }
                else
                {
                    qCritical() << "Can't prepare device";
                }
            }
        }
        else
        {
            mediaPlayer->stop();
            showToast(QString("This version is limited to playing %1 tracks. Please consider buying PRO version").arg(c_track_limit));
        }
    }

    void vk_chat_contoller::play(QString trackURL, QString artist, QString title)
    {
        np->setNextEnabled(true);
        np->setPreviousEnabled(true);
        if (m_playedTracks < c_track_limit)
        {
            if (mediaPlayer->sourceUrl() != trackURL)
            {
                mediaPlayer->setSourceUrl(trackURL);
                if ( mediaPlayer->prepare() == MediaError::None)
                {
                    m_playedTracks++;
                    m_songTitle = QString("%1 - %2").arg(artist, title);
                    // Integrate to now playing
                    bb::multimedia::MediaError::Type result = np->acquire();
                    qDebug() << "Acquire result:" << result;

                    emit songTitleChanged(m_songTitle);
                }
                else
                {
                    qCritical() << "Can't prepare device";
                }
            }
        }
        else
        {
            mediaPlayer->stop();
            showToast(QString("This version is limited to playing %1 tracks. Please consider buying PRO version").arg(c_track_limit));
        }
    }

    QString vk_chat_contoller::getSongTitle()
    {
        return m_songTitle;
    }

    void vk_chat_contoller::togglePlayback()
    {
//        mediaPlayer.
//        if mediaPlayer.
    }

    bb::cascades::GroupDataModel* vk_chat_contoller::getMusicModel() const
    {
        if (m_musicModel->isEmpty())
        {
            // Otherwise there would be infinite loop
            updateMusicModel();
//            _vkapi->vk_get_my_music();
        }
        return this->m_musicModel;
    }

    void vk_chat_contoller::hideVirtualKeyboard()
    {
        virtualkeyboard_hide();
    }

    void vk_chat_contoller::onMusicUpdated(const QVariant& list)
    {
        QVariantList testList = list.value<QVariantMap>().value("items").value<QVariantList>();
        bool messageInserted = false;
        qDebug() << "vk_chat_contoller::onMusicUpdated";
        if (testList.isEmpty())
        {
            // For popular list returned as plain list ...
            testList = list.value<QVariantList>();
        }
        QListIterator<QVariant> i(testList);
        i.toFront();
        while (i.hasNext())
        {
            const QVariant &v = i.next();
            if (!v.toMap().value("url").isNull())
            {
                VKMusic* newMusic = new VKMusic(v, this);
                m_musicModel->insert(newMusic);
                m_global_music_container->insert(newMusic->getId(), newMusic);
                messageInserted = true;
            }
        }
        if (messageInserted)
            emit musicUpdated(m_musicModel);
    }

    void vk_chat_contoller::onDialogsUpdated(const QVariant& list)
    {
        // TODO :: Make proper
        m_dialogsModel->clear();
        qDebug() << list;
        QVariantList testList = list.value<QVariantMap>().value("items").value<QVariantList>();
        qDebug() << "parsing dialogs list:" << testList;
//        QVariantList testList = list.value<QVariantList>();
        QStringList missingContacts;
        qDebug() << testList;
        bool messageInserted = false;
        uint l_fromID;
        QListIterator<QVariant> i(testList);
        i.toBack();
        while (i.hasPrevious())
        {
            const QVariant &v = i.previous();
            qDebug() << "Supplying:" << v;
//            if (v.type() != QMetaType::QVariantMap) {
//                qDebug() << "Skipping garbage";
//                continue;
//            }
            Vkmessage* newMessage = new Vkmessage(v, m_myInfo->getContactID(), this);
            l_fromID = newMessage->getFromID();
            // Insert with proper cache key
            if (newMessage->getOut()) { l_fromID = newMessage->getUserID(); }

//            qDebug() << "Message in dialog:" << newMessage->getUserID() << newMessage->getFromID();
            if (!this->m_global_chat_container->value(l_fromID) )
            {
                // No history yet.  Need to create new container
                QMap<uint, Vkmessage*>* _newCache = new QMap<uint, Vkmessage*>();
                // TODO :: Refactor
                this->m_global_chat_container->insert(l_fromID, _newCache);
            }
            // Check if we have person info. It might be message from someone who's not in our contact list

            if (!m_global_contactsCache->value(l_fromID) && l_fromID != 0)
            {
                qDebug() << "Fetch missing contact ID for" << l_fromID;
                missingContacts << QString::number(l_fromID);
            }

            m_global_chat_container->value(l_fromID)->insert(newMessage->getMessageID(), newMessage);

            m_dialogsModel->insert(newMessage);
            // TODO :: For now
            messageInserted = true;
        }

        if (missingContacts.count() > 0)
        {
            _vkapi->vk_get_users_info(missingContacts);
        }

        if (messageInserted)
                   emit dialogsUpdated(m_dialogsModel);
    }

    bb::multimedia::MediaPlayer* vk_chat_contoller::getMediaPlayer() const
    {
        return this->mediaPlayer;
    }

    void vk_chat_contoller::showToast(QString message)
    {
        SystemToast* myToast = new SystemToast(this);
        myToast->setBody(message);
        myToast->show();
    }

    Vkcontact* vk_chat_contoller::getMyInfo() const
    {
        return m_myInfo;
    }

    void vk_chat_contoller::onNowPlayingPlay()
    {
        qDebug() << "NP requested play";
        mediaPlayer->play();
    }

    void vk_chat_contoller::onNowPlayingPause()
    {
        qDebug() << "NP requested pause";
        mediaPlayer->pause();
    }

    void vk_chat_contoller::onAwake()
    {
        // Remove all notifications from hub
        Notification* pNotification = new Notification();
        pNotification->deleteAllFromInbox();
    }

    void vk_chat_contoller::onPlaybackCompleted()
    {
    }

    //    void vk_chat_contoller::onMediaStateChanged(bb::multimedia::MediaState::Type mediaState)
//    {
//        np->setMediaState(mediaState);
//    }

    void vk_chat_contoller::showVirtualKeyboard()
    {
        virtualkeyboard_show();
    }

    void vk_chat_contoller::setCurrentCommunityID(const uint communityID)
    {

        _currentCommunity = communityID;
        m_currentCommunity = m_communityCache->value(communityID);
        qDebug() << " setting community:" << m_currentCommunity->getId() << _currentCommunity << m_currentCommunity->getName();
        _vkapi->vk_get_wall(communityID, true);
    }

    void vk_chat_contoller::unsetCommunity()
    {
        _currentCommunity = -1;
    }

    void vk_chat_contoller::onNowPlayingAcquired()
    {
        onSongTitleChanged(m_songTitle);
        mediaPlayer->play();
    }

    void vk_chat_contoller::onSongTitleChanged(QString newTitle)
    {
        QVariantMap metadata2;
        QStringList metadata = newTitle.split(" - ", QString::SkipEmptyParts, Qt::CaseInsensitive);

        metadata2[MetaData::Title] = metadata.at(1);
        metadata2[MetaData::Artist] = metadata.at(0);
        metadata2[MetaData::Album] = "imVK";
        qDebug() << "Setting now playing metadata:" << metadata2;

        np->setMetaData(metadata2);

        np->setIconUrl(QUrl(QString("file://%1/app/native/assets/img/icon_110.png").arg(QDir::currentPath())));
    }

    void vk_chat_contoller::onNowPlayingRevoked()
    {
        qDebug() << "NP revoked. stopping";
        mediaPlayer->stop();
    }

    QByteArray vk_chat_contoller::encodeQString(const QString& toEncode) const
    {
        return toEncode.toUtf8();
    }

    void vk_chat_contoller::invoke(const QString& target, const QString& action,
            const QString& mimetype, const QString& uri)
    {
        InvokeRequest request;

        request.setTarget(target);
        request.setAction(action);

        if (target == QLatin1String("com.rim.bb.app.facebook")) {
                QVariantMap payload;

                if (!uri.isEmpty()) {
                        payload["object_type"] = mimetype;
                        payload["object_id"] = uri;
//                } else {
//                        // Open the BlackBerry North America page by default
//                        payload["object_type"] = "page";
//                        payload["object_id"] = "328506290597521";
                }

                request.setMetadata(payload);
        } else {
            request.setUri(uri);
        }

        m_invokeManager->invoke(request);
    }

    void vk_chat_contoller::sendImage(QString filename)
    {
        qDebug() << "vk_chat_contoller::sendImage : Sending image to:" << QString::number(_currentContactID) << ". Filename:" << filename;
        _vkapi->vk_send_photo(_currentContactID, filename);
    }

    void vk_chat_contoller::sendCurrentLocation(QString lattitude, QString longtitude)
    {
        _vkapi->vk_send_message(_currentContactID, "", "", lattitude, longtitude);
    }

    void vk_chat_contoller::savePreferredTheme(int newStyle)
    {
        if (!m_Settings)
        {
            m_Settings = new QSettings("Kartun", "imVKService", this);
        }
        m_Settings->setValue("app/Theme", newStyle);
        m_Settings->sync();
        qDebug() << "Saving preferred theme:" << newStyle;
    }

    void vk_chat_contoller::onEqChanged(bb::multimedia::EqualizerPreset::Type equalizerPreset)
    {
        if (!m_Settings)
        {
            m_Settings = new QSettings("Kartun", "imVKService", this);
        }
        m_Settings->setValue("app/EQ", equalizerPreset);
        m_Settings->sync();
        qDebug() << "Saving preferred EQ:" << equalizerPreset;
    }

    void vk_chat_contoller::addSongToMyList(uint trackID, uint ownerID)
    {
        _vkapi->vk_add_to_my_music(trackID, ownerID);
    }

    void vk_chat_contoller::deleteSongFromMyList(uint trackID, uint ownerID)
    {
        _vkapi->vk_delete_from_my_music(trackID, ownerID);
    }

    void vk_chat_contoller::postToCurrentCommunity(QString text)
    {
        _vkapi->vk_post_to_wall(_currentCommunity, text, true);
    }

    int vk_chat_contoller::getContactsCount() const
    {
        return this->m_contactsModel->size();
    }

    // IPC related functions
    void vk_chat_contoller::initIPC()
    {
        qDebug() << "imVK UI:: initIPC";
        bool result = connect(m_clientSocket, SIGNAL(connected()), this, SLOT(connected()));
        Q_ASSERT(result);
        Q_UNUSED(result);
        connectToServer();
    }

    void vk_chat_contoller::connectToServer()
    {
        qDebug() << "imVK card: connecting to server socket";
        if (!m_clientSocket->isOpen()) {
                qDebug() << "imVK card: no socket opened. Trying to open one";
                m_clientSocket->connectToHost(QHostAddress::LocalHost, m_port);
                qDebug() << "Is opened:" << m_clientSocket->isOpen();
                bool ok = connect(m_clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
                Q_ASSERT(ok);
                ok = connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
                Q_ASSERT(ok);
                Q_UNUSED(ok);
            } else {
                qDebug() << "imVK card: socket already opened.";
                connected();
            }
    }

    void vk_chat_contoller::readyRead()
    {
        QByteArray ba = m_clientSocket->readAll();
        qDebug() << "imVK card: received data from headless server: " << ba;
        qDebug() << "imVK card: Forwarding to vkapi";
        _vkapi->lps_replied(ba);
    }

    void vk_chat_contoller::connected()
    {
        qDebug() << "imVK card: connected to headless service.";
//        m_clientSocket->write(QByteArray::number(m_nextMove));
//        m_clientSocket->flush();
//        select(m_nextMove, -1);
    }

    void vk_chat_contoller::disconnected()
    {
        qDebug() << "imVK card: disconnected from headless service.";
    }


} /* namespace kartun_vkchat */
