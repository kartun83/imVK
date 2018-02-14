/*
 * VKmessageattachment.hpp
 *
 *  Created on: Aug 23, 2015
 *      Author: Kartun
 */

#ifndef VKMESSAGEATTACHMENT_HPP_
#define VKMESSAGEATTACHMENT_HPP_

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QFile>
#include <QDir>
//#include <QtCore/QFileInfo>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
//#include <QtNetwork/QNetworkAccessManager>
namespace kartun_vkchat
{

    class Vkattach: public QObject
    {
        Q_OBJECT

        Q_PROPERTY(int message_id READ getMessageID CONSTANT);
        // TODO :: Convert to enum
        Q_PROPERTY(uint contentType   READ getMediaType CONSTANT);
        Q_PROPERTY(QString body READ getMediaURL  NOTIFY urlChanged);
        // Required for the datamodel proper sorting and merging with the message body
//        Q_PROPERTY(uint date READ getTimestamp CONSTANT);
        Q_PROPERTY(bool out READ isOut CONSTANT);
        // Properties for specific attachment types, as I don't want to create interface ... yet
        Q_PROPERTY(QString artist READ getArtist CONSTANT);
        Q_PROPERTY(QString title  READ getTitle CONSTANT);
        Q_PROPERTY(QString description READ getDescription CONSTANT);
        Q_PROPERTY(uint duration  READ getDuration CONSTANT);

        public:
            // TODO :: rename to mediatype and replace everywhere
            typedef enum {
                _request_undefined = -1,
                _request_avatar  = 0,
                _request_media = 1,
    //            _request_sendMessage = 2,
            } requestType;

            // http://vk.com/dev/attachments_m
            typedef enum {
                _media_text  = 0,
                _media_photo = 1,
                _media_doc   = 2,
                _media_video = 3,
                _media_audio = 4,
                _media_sticker = 6,
                _media_wall   = 5,
                _media_wall_reply = 7,
                _media_unknown
            } mediaType;

            typedef enum {
                _size64      = 64,
                _size128     = 128,
                _size256     = 256,
                _size352     = 352,
                _size512     = 512,
            } stikerSizes;

            Vkattach(int message_id, bool out, uint mediaType, QString mediaURL, uint index = 0, QObject *parent = 0);
            // This one for parsing attachments from history
            Vkattach(int message_id, bool out, const QVariant &json, QObject *parent = 0);
            // This one for parsing attachments from LPS
            Vkattach(int message_id, bool out, const QVariantList &json, QObject *parent = 0);
            Vkattach(const Vkattach &other,  QObject *parent = 0);
            explicit Vkattach(QObject *parent = 0) : QObject(parent){}
            virtual ~Vkattach();
//        static const int _media_photo = 1;
//        static const int _media_doc   = 2;
//        static const int _media_video = 3;
//        static const int _media_audio = 4;

            uint getMediaType() const;
            QString getMediaURL() const;
            int getMessageID() const;
            uint getTimestamp() const;
            bool isLocal() const;
            //void setParent();
            bool isOut() const;
        Q_SIGNALS:
            void urlChanged(QString newUrl);
        public Q_SLOTS:
            void processMediaDownloaded(QNetworkReply *reply);
            uint getIdx() const;
        void setIdx(uint idx);
        const QString& getMediaUrlLarge() const;
        const QString& getArtist() const;
        const QString& getDescription() const;
        uint getDuration() const;
        const QString& getTitle() const;
const QString& getMediaUrl() const;
        const QString& getVideoAccessKey() const;
        int getVideoId() const;

        private:
            uint _mediaType;
            uint    _idx;   // Index of this attachment in parent message (also in model)
            uint    _timestamp;
            int     _message_id;
            bool    _out;
            QString _mediaURL;
            QString _mediaURL_large;
            QString _localURL;
            // Attributes for specific attachment types
            QString m_artist;
            QString m_title;
            QString m_description;
            uint    m_duration;
            int     m_videoID;
            QString m_videoAccessKey;
            QNetworkAccessManager *manager2;
            QMap<QString, int> attachmentTypeMap;
            void constructorGeneral();
            bool downloadMedia(int attachmentType, QString url, QString stikerPackID = "");
            void createDirectories();
            QString _cameraPath, _videosPath, _documentsPath, _audioPath, _attachmentsPath, _stickersPath;
    };

} /* namespace kartun_vkchat */

Q_DECLARE_METATYPE( kartun_vkchat::Vkattach *);

#endif /* VKMESSAGEATTACHMENT_HPP_ */
