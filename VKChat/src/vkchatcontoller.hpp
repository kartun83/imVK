/*
 * vkchatcontoller.hpp
 *
 *  Created on: May 18, 2015
 *      Author: Kartun
 */

#ifndef VKCHATCONTOLLER_HPP_
#define VKCHATCONTOLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ArrayDataModel>
#include <bb/multimedia/MediaPlayer>
#include <bb/multimedia/NowPlayingConnection>
#include <bb/system/InvokeManager>
#include "DataModel/MessageDataModel.h"
#include "DataModel/DialogDataModel.hpp"
#include "vkauth.hpp"
#include "vkmessage.hpp"
#include "Vkcontact.hpp"
#include "VKWall.hpp"
#include "VKMusic.hpp"
#include "VKCommunity.hpp"
#include "VKWallComment.hpp"

#include <bb/cascades/ThemeSupport>
#include "bps/virtualkeyboard.h"
// IPC
#include <QtNetwork/QTcpSocket>

using namespace bb::cascades;


namespace kartun_vkchat
{

    class vk_chat_contoller: public QObject
    {
        Q_OBJECT

        // Properties for QML access
        Q_PROPERTY(bb::cascades::GroupDataModel *contactsModel READ contactsModel NOTIFY contactsUpdated);
        Q_PROPERTY(int countactCount READ getContactsCount NOTIFY contactCountChanged);
        Q_PROPERTY(bb::cascades::GroupDataModel *chatModel READ chatModel NOTIFY chatUpdated);
        Q_PROPERTY(bb::cascades::GroupDataModel *musicModel READ getMusicModel NOTIFY musicUpdated );
        Q_PROPERTY(bb::cascades::GroupDataModel *dialogsModel READ getDialogsModel NOTIFY dialogsUpdated );
        Q_PROPERTY(bb::cascades::GroupDataModel *communitiesModel READ getCommunitiesModel NOTIFY communitiesUpdated );
        Q_PROPERTY(bb::cascades::GroupDataModel *wallModel READ getWallModel NOTIFY wallChanged);
        Q_PROPERTY(bb::cascades::GroupDataModel *wallCommentsModel READ getWallCommentsModel NOTIFY wallCommentsChanged);
        Q_PROPERTY(bb::cascades::DataModel* chatModel2 READ getChatModel2 CONSTANT  );
        Q_PROPERTY(QString cameraPath READ getCameraPath CONSTANT);
        Q_PROPERTY(Vkcontact *currentContact READ getCurrentContact CONSTANT);
        Q_PROPERTY(VKCommunity *currentCommunity READ getCurrentCommunity CONSTANT);
        Q_PROPERTY(Vkcontact *myInfo READ getMyInfo NOTIFY myInfoUpdated);
        Q_PROPERTY(Vkmessage* latestMessage READ getLatestMessage NOTIFY latestMessageUpdated );
        Q_PROPERTY(bb::multimedia::MediaPlayer* mediaPlayer READ getMediaPlayer CONSTANT );
        Q_PROPERTY(QString songTitle READ getSongTitle NOTIFY songTitleChanged);
        //Q_PROPERTY(vk_api::my_info contactStatus READ getCurrentContactStatus NOTIFY currentContactStatusUpdated CONSTANT);
        //Q_PROPERTY(vk_api::my_info currentContact READ getCurrentContactDetails NOTIFY currentContactChanged CONSTANT);
    private:
    public:
        enum ContactType{ Contact = 1, Community = 2 };
        typedef enum {
            _request_undefined = -1,
            _request_avatar  = 0,
            _request_media = 1,
//            _request_sendMessage = 2,
        } requestType;
        // Constants for media download
        typedef enum {
            _media_text  = 0,
            _media_photo = 1,
            _media_doc   = 2,
            _media_video = 3,
            _media_audio = 4,
        } mediaType;
        // Attachments
        typedef enum {
            _attachment_unknown = 0,
            _attachment_photo = 1,
            _attachment_video = 2,
            _attachment_audio = 3,
            _attachment_doc   = 4,
            _attachment_wall  = 5,
            _attachment_sticker = 6,

        } attachmentType;

        typedef enum {
            _deact_deleted = 1,
            _deact_banned  = 2,
        } deactivationReason;

        ////////
        vk_api* _vkapi;
        // Accessors
        bb::cascades::GroupDataModel* contactsModel() const;
        bb::cascades::GroupDataModel* chatModel() const;
        kartun_vkchat::MessageDataModel* getChatModel2() const;
        bb::cascades::GroupDataModel* getMusicModel() const;
        bb::cascades::GroupDataModel* getDialogsModel() const;
        bb::cascades::GroupDataModel* getCommunitiesModel() const;
        bb::cascades::GroupDataModel* getWallModel() const;
        bb::cascades::GroupDataModel* getWallCommentsModel() const;
        int getContactsCount() const;
        QString getCameraPath() const;
        Vkcontact* getCurrentContact() const;
        Vkcontact* getMyInfo() const;
        VKCommunity* getCurrentCommunity() const;
        Vkmessage* getLatestMessage() const;

        // Worker methods
        void updateContactsModel() const;
        void updateChatModel() const;
//        void updateMusicModel() const;
        void updateDialogsModel() const;
        void updateCommunities() const;
        void getPaths();
        bb::multimedia::MediaPlayer* getMediaPlayer() const;

        void processAvatarDownloaded(QNetworkReply *reply);
        void processMediaDownloaded(QNetworkReply *reply);

        // Return true, if local avatar available
        bool downloadAvatar(QString url, ContactType contactType, uint userID);
    private:
        // Needed, as c++ doesn't allow to build switches on strings
        QMap<QString, int> attachmentTypeMap;
        QMap<QString, int> deactivatedReason;


        // Avatar and media downloader
        QNetworkAccessManager *manager2;
        bool m_authorized;

        // The property values
        bb::cascades::GroupDataModel* m_contactsModel;
        // TODO :: Remove
        bb::cascades::GroupDataModel* m_chatModel;
        bb::cascades::GroupDataModel* m_musicModel;
        bb::cascades::GroupDataModel* m_communitiesModel;
        bb::cascades::GroupDataModel* m_wallModel;
        bb::cascades::GroupDataModel* m_wallCommentsModel;
//        bb::cascades::GroupDataModel* m_dialogsModel;
        DialogDataModel* m_dialogsModel;
        kartun_vkchat::MessageDataModel* m_chatModel2;
        Vkcontact* m_currentContact;
        Vkcontact* m_myInfo;
        VKCommunity* m_currentCommunity;
        Vkmessage* m_latestMessage;
        bb::multimedia::MediaPlayer* mediaPlayer;

        QString _cameraPath, _videosPath, _documentsPath, _audioPath;
        QString _avatarPath;
        QString _attachmentsPath;
        QString m_songTitle;
        uint    m_playedTracks;
        uint    c_track_limit;
        // This map would contain all messages for all chats, so it would be possible to
        // 1. Append new messages for given contact id
        // 2. Initialize new chat if no messages have been downloaded yet
        // userID - key
        QMap<uint, QMap<uint, Vkmessage*>*>* m_global_chat_container;
        // Cache for music items
        QMap<uint, VKMusic*>* m_global_music_container;
        // Cache for unfinished messages. I.e. not in read state, download pending etc
        //QMap<uint, Vkmessage*>* m_global_messageCache;
        // Cache for contacts. As Group data model allow access only by sorting keys and fully qualified keys
        QMap<uint, Vkcontact*>* m_global_contactsCache;
        // Cache for communities
        QHash<uint, VKCommunity*>* m_communityCache;
        // Now playing integration
        bb::multimedia::NowPlayingConnection* np;
        // Invokation integration
        bb::system::InvokeManager* m_invokeManager;

        void downloadMedia(QString url, int fromID, int mediaType, int messageID);
        uint _currentContactID;
        uint _currentCommunity;
        uint m_playlistCounter;  // Media player doesn't allow to refresh playlists
        QString vk_app_id;
        void avatarLocallyAvailable(uint userID, ContactType contactType, QString localFileName);
        QString getSongTitle();
        void showToast(QString message);
        QSettings*   m_Settings;
        void storeAccountSettings(QString &accessToken, QString &userId);

        // IPC related
        // The port on which to establish socket connection with server.
        int m_port;
        // Place holder for the communication socket
        QTcpSocket *m_clientSocket;
        // Method to establish the socket connection with the UI server socket.
        void connectToServer();

    Q_SIGNALS:
        void contactsUpdated(bb::cascades::GroupDataModel* newContactList) const;
//        void chatUpdated(bb::cascades::GroupDataModel* newChat) const;  // This connected to scroll to bottom on view
        void chatUpdated(bb::cascades::DataModel* newChat) const;
        void musicUpdated(bb::cascades::DataModel* newMusic) const;
        void dialogsUpdated(bb::cascades::GroupDataModel* newDialogs) const;
        void currentContactChanged(vk_api::my_info currentContact) const;
        void communitiesUpdated(bb::cascades::GroupDataModel* newCommunities) const;
        void wallChanged(bb::cascades::GroupDataModel* newWall) const;
        void wallCommentsChanged(bb::cascades::GroupDataModel* newWallComments) const;
        void contactCountChanged(int newContactsCount) const;
        // Signals for media downloaded
        void photoDownloaded(int fromID, int mediaType, int messageID, QString filename);
        void currentContactStatusUpdated(vk_api::my_info contactStatus) const;
        void message_parsed(QString messageID);
        void myInfoUpdated(Vkcontact* newInfo);
        void latestMessageUpdated(Vkmessage* latestMessage);
        void networkErrorOccured(QNetworkReply::NetworkError networkError, QString networkErrorStr);
        void authTokenReceived();
        void songTitleChanged(QString newTitle);

    public Q_SLOTS:
        // For signals from vk_api
        void onContactListUpdated(const QVariant &list);
        void myInfoUpdated(const QVariant &list);
        void chatListUpdated(uint fromID,const  QVariant &list);
        void chatListUpdatedLPS(const QVariant &list);
        void onMusicUpdated(const QVariant &list);
        void onDialogsUpdated(const QVariant &list);
        void onUsersInfoUpdated(const QVariant &list);
        void onCommunitiesUpdated(const QVariant &list);
        void onWallDownloaded(const QVariant &list);
        void onWallCommentsDownloaded(const QVariant &list);
        void onNewMessageReceived(vk_api::new_message_struct newMessage, const QVariant &attachments);
        void onLatestMessageUpdated(Vkmessage* latestMessage);
        // For media and avatars downloading
        void onMediaDownloaded(QNetworkReply *reply);
        void onUserOnlineChanged(const uint &userID, const bool &online);
        // Now playing integration
        void onNowPlayingAcquired();
        void onNowPlayingRevoked();
        void onNowPlayingPlay();
        void onNowPlayingPause();
        void onEqChanged(bb::multimedia::EqualizerPreset::Type equalizerPreset);
//        void onNowPlayingNext();
//        void onNowPlayingPrev();
        void onSongTitleChanged(QString newTitle);
        void onPlaybackCompleted();
        void onAwake();
//        void onMediaStateChanged(bb::multimedia::MediaState::Type mediaState);  // For now playing updates
//        void avatarLocallyAvailable(uint userID, QString localFileName);
        // IPC
          // This method is invoked through the signal/slot mechanism as well, when the socket is ready to read data.
        void readyRead();
        // This method is invoked when the socket connects to it's headless server socket for communication.
        void connected();

        // This method is invoked when the socket disconnects.
        void disconnected();

        // For attachments processing
        void attachmentPhotoDownloaded(int fromID, int mediaType, int messageID, QString filename);
    public:
        vk_chat_contoller(QObject* parent = 0);
        virtual ~vk_chat_contoller();
        // Invokable methods
        Q_INVOKABLE void setAuthData(QString accessToken, QString userId, int expiriesIn);
        Q_INVOKABLE void setCurrentContactID(const uint contactID );
        Q_INVOKABLE void setCurrentCommunityID(const uint communityID);
        Q_INVOKABLE void unsetCurrentContact();
        Q_INVOKABLE void unsetCommunity();
        Q_INVOKABLE void sendMessageToCurrentContact(QString messageText);
        Q_INVOKABLE void sortContacts(uint sortType);
        Q_INVOKABLE QString getContactFullName(uint userID);
        Q_INVOKABLE Vkcontact* getContact(uint userID);
        Q_INVOKABLE void searchMusic(QString searchTerm);
        Q_INVOKABLE void addSongToMyList(uint trackID, uint ownerID);
        Q_INVOKABLE void deleteSongFromMyList(uint trackID, uint ownerID);
        Q_INVOKABLE void getPopularMusic();
        Q_INVOKABLE void getRecommendedMusic(uint trackID = 0, uint ownerID = 0);
        Q_INVOKABLE void updateMusicModel() const;
        Q_INVOKABLE void play(uint trackID);
        Q_INVOKABLE void play(QString trackURL, QString artist, QString title);
        Q_INVOKABLE void togglePlayback();
        Q_INVOKABLE void sendImage(QString filename);
        Q_INVOKABLE void sendCurrentLocation(QString lattitude, QString longtitude);
        Q_INVOKABLE void postToCurrentCommunity(QString text);
        Q_INVOKABLE void fetchWallComments(int wallID, uint post_id, bool isCommunity = false);
        Q_INVOKABLE QByteArray encodeQString(const QString& toEncode) const;
        Q_INVOKABLE void invoke(const QString &target, const QString &action, const QString &mimetype, const QString &uri);
//        Q_INVOKABLE void setEQPreset(int newPreset);
        //Q_INVOKABLE bool isVirtualKeyboardVisible();
        Q_INVOKABLE void hideVirtualKeyboard();
        Q_INVOKABLE void showVirtualKeyboard();
        Q_INVOKABLE void savePreferredTheme(int newStyle);
        // IPC
        void initIPC();
    };

} /* namespace kartun_timetrack */

#endif /* VKCHATCONTOLLER_HPP_ */
