/*
 * VKmessageattachment.cpp
 *
 *  Created on: Aug 23, 2015
 *      Author: Kartun
 */

#include "VKmessageattachment.hpp"
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

namespace kartun_vkchat
{

//    Vkattach::Vkattach(int message_id, uint timestamp, bool out, uint mediaType, QString mediaURL, QObject *parent ) : QObject(parent)
    Vkattach::Vkattach(int message_id, /* uint timestamp, */bool out,  uint mediaType, QString mediaURL, uint index, QObject *parent ) : QObject(parent)
    {
        // TODO Auto-generated constructor stub
        this->_message_id = message_id;
        this->_mediaType = mediaType;
        this->_mediaURL  = mediaURL;
        this->_idx        = index;
//        this->_timestamp = timestamp;
        this->_out       = out;
        // TODO :: add checks
        constructorGeneral();
        downloadMedia(this->_mediaType, this->_mediaURL);
    }

    Vkattach::Vkattach(int message_id, bool out, const QVariantList& json, QObject* parent)
    {
        this->_message_id = message_id;
        this->_out       = out;
        constructorGeneral();
        // TODO :: Confirm
        if (json.count() == 3)
        {
            QString field1 = json.at(0).toString();
            QString field2 = json.at(1).toString();
            QString field3 = json.at(2).toString();
            qDebug() << "Received attachment type:" << field3;
            if (field3.isEmpty())
            {
                qCritical() << "Empty attachment received!!!!" << json;
            }
            switch (attachmentTypeMap.value(field3)) {
                case _media_photo:

                    break;
                case _media_sticker:
                        _mediaType = _media_sticker;
                        // http://vk.com/images/stickers/2052/128b.png
                        // TODO :: Add settings for this
                        // TODO :: Better heuristics
                        _mediaURL  = QString("%1/%2/%3").arg("http://vk.com/images/stickers",field1,"128b.png");
                            break;
                default:
                    break;
            }
            qDebug() << "Attachment from LPS:" << _mediaURL;
            if (_mediaType == _media_photo || _mediaType == _media_sticker)
            {
                downloadMedia(this->_mediaType, this->_mediaURL, field2);
            }
        }
    }

    void Vkattach::constructorGeneral()
    {
        // Fill attachment type map, as c++ doesn't allow to build switches on strings
        attachmentTypeMap.insert("photo",  _media_photo);
        attachmentTypeMap.insert("video",  _media_video);
        attachmentTypeMap.insert("audio",  _media_audio);
        attachmentTypeMap.insert("doc",    _media_doc);
        attachmentTypeMap.insert("wall",   _media_wall);
        attachmentTypeMap.insert("wall_reply",_media_wall_reply);
        attachmentTypeMap.insert("sticker",_media_sticker);
        attachmentTypeMap.insert("link", _media_link);
        attachmentTypeMap.insert("posted_photo", _media_posted_photo);
        attachmentTypeMap.insert("unknown",_media_unknown);
        // Paths
//        QString workingDir = QDir::currentPath();
//        // TODO :: Make proper
//        _stickersPath = QString("file://" + workingDir + "/data/stickers/");
//        _cameraPath     = QString("file://" + workingDir + "/shared/photos/imVK/");
//        _documentsPath  = QString("file://" + workingDir + "/shared/documents/imVK/");
//        _videosPath     = QString("file://" + workingDir + "/shared/videos/imVK/");
        createDirectories();
        manager2 = new QNetworkAccessManager(this);
        bool connect_result = connect(manager2, SIGNAL(finished(QNetworkReply*)),
                                      this, SLOT(processMediaDownloaded(QNetworkReply*)));
        Q_ASSERT(connect_result);
    }

    Vkattach::Vkattach(const Vkattach& other, QObject* parent) : QObject(parent)
    {
        this->_idx        = other._idx;
        this->_message_id = other._message_id;
        this->_mediaType  = other._mediaType;
        this->_mediaURL   = other._mediaURL;
        constructorGeneral();
        downloadMedia(this->_mediaType, this->_mediaURL);
    }

    Vkattach::Vkattach(int message_id, bool out, const QVariant& json, QObject* parent) : QObject(parent)
    {
        this->_message_id = message_id;
        this->_out = out;
        constructorGeneral();
        QMap<QString, QVariant> attachmentsMap = json.toMap();
        this->_mediaType = attachmentTypeMap.value(attachmentsMap.value("type").toString());

        QVariantMap attachmentParseMap;


        switch (_mediaType) {
            case _media_photo :
                attachmentParseMap = attachmentsMap.value("photo").value<QVariantMap>();
                _mediaURL = attachmentParseMap.value("photo_604").toString();
                _mediaURL_large = attachmentParseMap.value("photo_1280").toString();
                if (_mediaURL_large.isEmpty()) { _mediaURL_large = _mediaURL; }
//                qDebug() << photoURL;
//                qDebug() << photoURL_large;
                break;
            case _media_video : attachmentParseMap = attachmentsMap.value("video").value<QVariantMap>();
                                qDebug() << "Parse map:" << attachmentParseMap;
                                m_title    = attachmentParseMap.value("title").toString();
                                m_duration = attachmentParseMap.value("duration").toUInt();
                                m_videoID  = attachmentParseMap.value("id").toInt();
                                _mediaURL  = attachmentParseMap.value("photo_320").toString();
                                m_videoAccessKey = attachmentParseMap.value("access_key").toString();
                                break;
            case _media_audio : attachmentParseMap = attachmentsMap.value("audio").value<QVariantMap>();
                                _mediaURL = attachmentParseMap.value("url").toString();
                                m_artist   = attachmentParseMap.value("artist").toString();
                                m_title    = attachmentParseMap.value("title").toString();
                                m_duration = attachmentParseMap.value("duration").toUInt();
                                break;
            case _media_doc   : _mediaURL = "Doc";   break;
            case _media_wall  : _mediaURL = "Wall";  break;
            case _media_sticker : _mediaURL = attachmentsMap.value("sticker").toMap().value("photo_128").toString(); break;
            // TODO:: Make translatable
            case _media_unknown:
            default: _mediaURL = "other";
                break;
        }
        qDebug() << "Attachment:" << attachmentsMap.value("type").toString() << ", mapped to:" << _mediaType;
        // Auto download stickers and photos
        if (_mediaType == _media_photo || _mediaType == _media_sticker || _mediaType == _media_video)
        {
            downloadMedia(this->_mediaType, this->_mediaURL);
        }
    }


    Vkattach::~Vkattach()
    {
        // TODO Auto-generated destructor stub
    }

    bool Vkattach::isLocal() const
    {
        QUrl fileInf(this->_mediaURL);
        bool result = false;
        QString _abc = fileInf.scheme();
        if ( fileInf.scheme() == "file" )
        {
            result = true;
        }
        return result;
    }

    uint Vkattach::getMediaType() const
    {
        return this->_mediaType;
    }

    QString Vkattach::getMediaURL() const
    {
        return this->_mediaURL;
    }

    int Vkattach::getMessageID() const
    {
        return _message_id;
    }

    uint Vkattach::getTimestamp() const
    {
        return this->_timestamp;
    }

    bool Vkattach::isOut() const
    {
        return _out;
    }

    uint Vkattach::getIdx() const
    {
        return _idx;
    }

    const QString& Vkattach::getMediaUrlLarge() const
    {
        return _mediaURL_large;
    }

    void Vkattach::setIdx(uint idx)
    {
        _idx = idx;
    }

    void Vkattach::createDirectories()
    {
        QString workingDir = QDir::currentPath();
        _cameraPath     = QString("file://" + workingDir + "/shared/photos/.imVK/");
        _documentsPath  = QString("file://" + workingDir + "/shared/documents/imVK/");
        _videosPath     = QString("file://" + workingDir + "/shared/videos/.imVK/");
        _stickersPath   = QString("file://" + workingDir + "/data/stikers/");

        QDir dir; // New QDir objects default to the application working directory.

        bool result = dir.mkpath("data/stikers");
        if(result)
        {
            qDebug() << "Stikers dir created";
        }
        else
        {
            qDebug() << "Stikers dir isn't created. Possibly exists";
        }


        result = dir.mkpath("/" + workingDir + "/shared/photos/.imVK/");
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

        result = dir.mkpath("/" + workingDir + "/shared/videos/.imVK/");
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

//        _avatarPath = QString("file://" + workingDir + "/data/avatars/");

//        QDir dir2; // New QDir objects default to the application working directory.
//        result = dir.mkpath("data/attachments");
//        if(result)
//        {
//            qDebug() << "Attachments dir created";
//        }
//        else
//        {
//            qDebug() << "Attachments dir isn't created. Possibly exists";
//        }
//        dir.mkpath("data/stickers");
//        _attachmentsPath = QString("file://" + workingDir + "/data/attachments/");
    }

    const QString& Vkattach::getArtist() const
    {
        return m_artist;
    }

    const QString& Vkattach::getDescription() const
    {
        return m_description;
    }

    uint Vkattach::getDuration() const
    {
        return m_duration;
    }

    const QString& Vkattach::getTitle() const
    {
        return m_title;
    }

    const QString& Vkattach::getMediaUrl() const
    {
        return _mediaURL;
    }

    const QString& Vkattach::getVideoAccessKey() const
    {
        return m_videoAccessKey;
    }

    int Vkattach::getVideoId() const
    {
        return m_videoID;
    }

    bool Vkattach::downloadMedia(int attachmentType, QString url, QString stikerPackID)
    {
        // Check, if we already have an avatar
       QFileInfo fileInf(url);
       QString fileName = fileInf.fileName();
       QString localFName;
       QString stikerpack_id;
       QFile   _meidaFile(QString(localFName +fileName));
       switch (attachmentType) {
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
           case _media_sticker:
                 stikerpack_id = fileInf.dir().dirName();
                 localFName = _stickersPath.mid(7).append(QString("\%1_").arg(stikerpack_id)); break;
           default:
                 qCritical() << "Unknown attachment type";
               break;
       }

       QFile   _mediaFile(QString(localFName +fileName));

       if ( _mediaFile.exists() )
       {
//           avatarLocallyAvailable(userID, _avatarFile.fileName());
           // TODO :: Make proper
            this->_localURL = "file://" + QString(localFName +fileName);
            this->_mediaURL = this->_localURL;
            emit urlChanged(this->_localURL);
           return true;
       }
       else
       {
           QNetworkReply* reply = manager2->get(QNetworkRequest(url));
           // TODO :: For sticker detect set
//           reply->setProperty("userID", userID);
           reply->setProperty("mediaType", attachmentType);
           if (attachmentType == _media_sticker)
           {
               reply->setProperty("stickerPackID", stikerPackID);
           }
       }
       return false;
    }

    void Vkattach::processMediaDownloaded(QNetworkReply *reply)
        {
            int mediaType = reply->property("mediaType").toInt();
//            int messageID = reply->property("messageID").toInt();
//            emit message_parsed(reply->property("messageID").toString());
//            int fromID = reply->property("fromID").toInt();
            QString localFName;
            QString stikerpack_id;
            QUrl url = reply->url();
            QFileInfo fileInf(url.toString());
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
                case _media_sticker:
                      stikerpack_id = fileInf.dir().dirName();
                      localFName = _stickersPath.mid(7).append(QString("\%1_").arg(stikerpack_id)); break;
                default:
                    break;
            }

           if ( reply->error() == QNetworkReply::NoError )
           {
               QByteArray buffer = reply->readAll();
               // Get filename from network reply
               QString fileName = fileInf.fileName();

               QFile   _mediaFile(QString(localFName +fileName));
               {
                   bool result;
                   result = _mediaFile.open(QIODevice::WriteOnly);
//                   qDebug() << _mediaFile.error() << _mediaFile.errorString();
                   Q_ASSERT(result);
                   Q_UNUSED(result);
                   _mediaFile.write(buffer);
                   //out << "This is a text file\n";
                   _mediaFile.close();
                   switch (mediaType) {
                       case _media_photo:
                             this->_localURL = "file://" + _mediaFile.fileName();
                             this->_mediaURL = this->_localURL;
                             emit urlChanged(this->_localURL);
                           break;
                       case _media_sticker:
                             this->_localURL = "file://" + _mediaFile.fileName();
                             this->_mediaURL = this->_localURL;
                             emit urlChanged(this->_localURL);
//                           emit photoDownloaded(fromID, mediaType, messageID, _mediaFile.fileName());
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
//               qDebug() << reply->errorString();
           }
           reply->deleteLater();
        }

}
/* namespace kartun_vkchat */
