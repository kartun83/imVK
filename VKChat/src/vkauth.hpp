/*
 * vkauth.h
 *
 *  Created on: May 10, 2015
 *      Author: Kartun
 */

#ifndef VKAUTH_H_
#define VKAUTH_H_

#define BIT0 1
#define BIT1 2
#define BIT2 4
#define BIT3 8
#define BIT4 16
#define BIT5 32
#define BIT6 64
#define BIT7 128

#include <QtCore/QObject>
#include <QTimer>
//#include <QObject>
//#include <bb/cascades/WebView>
#include <bb/network/PushService>



namespace kartun_vkchat
{

    class vk_api: public QObject
    {

        Q_OBJECT
        Q_CLASSINFO("Version", "1.0.0")
        Q_CLASSINFO("Author", "Alexey Tveritinov")
    public:
        // public types
        struct long_poll_server_data{
                    QString key;
                    QString server;
                    QString ts;
                };
        struct new_message_struct{
            // $message_id,$flags,$from_id,$timestamp,$subject,$text,$attachments
            int     message_id;
            uint    from_id;
            uint    user_id;
            uint    timestamp;
            bool    out;
            bool    read;
            QString subject;
            QString text;
//            int     messageContentType;
            // Reserved for attachments
        };


        struct my_info{
            QString userID;
            QString first_name;
            QString last_name;
            QString screen_name;
            QString localAvatar;
        };

    private:
       // QString auth_url = "https://oauth.vk.com/authorize?client_id=&redirect_uri=https://oauth.vk.com/blank.html&scope=friends,messages&display=mobile";
        QString _token;
        QString _userID;
        QString _appID;
        //QTimer _timer();
        QString _url;
        QString _api_version;
        QString _max_msg_id;
        uint    _lps_wait;
        bool    _lps_request_active;
        bb::network::PushService* m_pushService;
        QTimer* _lps_watchdog;
        long_poll_server_data _lps_data;

        QNetworkConfigurationManager* m_pNetConfigMngr;
        QNetworkAccessManager* m_pNetAccessMngr;
        QNetworkAccessManager* lps_NetworkManager;
        void vk_parse_error(const QByteArray &buffer);
        void vk_fetch_long_poll();
        void vk_track_statistics();
        void vk_upload_photo(int userID, QString photoPath, QVariant &list);
        void vk_save_message_photo(QString uploadServer, int userID, QVariant &list);
        void vk_send_message_with_photo(int userID, QVariant& list);
        void perform_signal_connections();

        // Constants for network requests types

        public:
            typedef enum {
                _request_undefined = -1,
                _request_authkey  = 0,
                _request_myInfo = 1,
                _request_sendMessage = 2,
                _request_setOnline = 3,
                _request_trackStatistics = 4,
                _request_getAllFriends  = 5,
                _request_getAllMessages  = 6,
                _request_getMessagesWithUser      = 7,
                _request_registerDevice     = 8,
                _request_startTyping        = 9,
                _request_sendStiker         = 10,
                _request_markMessagesRead   = 11,
                _request_getDialogs         = 12,
                _request_getLongPollHistory = 13,
                _request_myMusic            = 14,
                _request_usersInfo          = 15,
                _request_getGroups          = 16,
                _request_getWall            = 17,
                _request_getUploadServer    = 18,
                _request_uploadToServer     = 19,
                _request_uploadSavePhoto    = 20,
                _request_audioGetPopular    = 21,
                _request_audioAddToMyList   = 22,
                _request_audioRemoveFromMyList   = 23,
                _request_wallPost           = 24,
                _request_wallGetComments    = 25,
            } requestType;
            typedef enum  {
                _lps_request_init = 1,
                _lps_request_poll = 2,
            }lpsRequest;

            typedef enum {
                _lps_reply_messageDeleted = 0,
                _lps_reply_flagsChanged   = 1,
                _lps_reply_flagsSet       = 2,
                _lps_reply_flagsReset     = 3,
                _lps_reply_newMessage     = 4,
                _lps_reply_incomingRead   = 6,
                _lps_reply_outgoingRead   = 7,
                _lps_reply_userOnline     = 8,
                _lps_reply_userOffline    = 9,
                _lps_reply_chatChanged    = 51,
                _lps_reply_userTypingInDialog     = 61,
                _lps_reply_userTypingInChat       = 62,
                _lps_reply_userCall               = 70,
                _lps_reply_unreadChanged          = 80,
            }lpsReply;

    Q_SIGNALS:
        void contactListUpdated(const QVariant &list);
        void chatUpdated(uint fromID,const  QVariant &list);
        void chatUpdatedLPS(const QVariant &list);
        void myInfoUpdated(const QVariant &list);
        void musicUpdated(const QVariant &list);
        void usersInfoUpdated(const QVariant &list);
        void groupsUpdated(const QVariant &list);
        void wallDownloaded(const QVariant &list);
        void wallCommentsDownloaded(const QVariant &list);
        void messageRead();
        void messageSent(const QVariant &list);
        void dialogsUpdated(const QVariant &list);
        void userOnlineChanged(const uint &userID, const bool &online);
        void newMessageReceived(vk_api::new_message_struct newMessage, const QVariant &attachments);
        void networkErrorOccured(QNetworkReply::NetworkError networkError, QString networkErrorStr);
    public Q_SLOTS:
        void onLPSReply(QNetworkReply *reply);
        void networkReplyReceived(QNetworkReply *reply);
        void networkErrorReceived(QNetworkReply::NetworkError networkError, QString networkErrorStr);
        // Indicates that message have been parsed. Required for LPS
        void message_parsed(QString message_id);
        void lpsWatchdogTimeout();
        void vk_connect_long_poll_server();
        // TODO :: Remove.
        // temporary, for testing HUB integration
//        void onNewMessageReceived(vk_api::new_message_struct newMessage);
    public:
        // dontConnectLPS - forbid LPS connect for foreground app, so everything would be
        // handled by headless
        vk_api(QString accessToken, QString userId, int expiriesIn, QString appID, bool dontConnectLPS);
        void vk_get_all_friends();
        void vk_get_online_friends();
        void vk_get_all_messages();
        void vk_get_dialogs();
        void vk_get_messages_with_user(int userID);
        void getLongPollHistory(QString ts);
        void vk_send_message(int userID, QString messageText, QString attachment = "", QString lattitude = "", QString longtitude = "");
        void vk_send_stiker(int userID, uint stickerID);
        void vk_messages_mark_read(int userID, int startMessageID);
        void vk_get_my_info();
        void vk_get_users_info(QStringList &users);
        void vk_get_my_music();
        void vk_get_popular_music();
        void vk_get_recommended_music(uint trackID = 0, uint ownerID = 0);
        void vk_add_to_my_music(uint audioID, uint ownerID);
        void vk_delete_from_my_music(uint audioID, uint ownerID);
        void vk_search_music(QString query);
        void vk_set_online(bool isOnline);
        void vk_send_photo(uint userID, QString photoPath);
        // TODO :: Add to preferences, consume battery
        void vk_set_typing();
        void vk_get_groups();
        void vk_get_wall(int wallID, bool isCommunity = false);
        void vk_post_to_wall(int wallID, QString messageText, bool isCommunity = false, QString attachment = "",
                             QString lattitude = "", QString longtitude = "", bool friends_only = false,
                             bool from_group = false);
        void vk_get_wall_comments(int wallID, uint post_id, bool isCommunity = false);

        QString deviceModel() const;
        QString deviceVersion() const;
        QString deviceID() const;
        // For testing hub integration
        void lps_replied(QByteArray buffer);
        static void unescapeXML(QString &inXML);
        virtual ~vk_api();
    private:
        void startPushService();
        void lps_initalized(QByteArray buffer);
//        void lps_replied(QByteArray buffer);
        void log_lps_error(QNetworkReply* reply);
    };

} /* namespace kartun_timetrack */

#endif /* VKAUTH_H_ */
