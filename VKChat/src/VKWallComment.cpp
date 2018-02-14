/*
 * VKWallComment.cpp
 *
 *  Created on: Dec 27, 2015
 *      Author: kartun
 */

#include <src/VKWallComment.hpp>

namespace kartun_vkchat
{

    VKWallComment::VKWallComment(const QVariant &json, QObject *parent) : QObject(parent)
    {
        // TODO Auto-generated constructor stub
        m_geo = NULL;
        m_attachments = NULL;
        QMap<QString, QVariant> responseMap = json.toMap();

        qDebug() << "Creating wall comment from json" << json;

        m_id = responseMap.value("id").toUInt();
        m_fromId = responseMap.value("from_id").toInt();
        m_text   = responseMap.value("text").toString();
        m_replyToComment = responseMap.value("reply_to_comment").toUInt();
        m_replyToUser = responseMap.value("reply_to_user").toInt();
        m_timestamp   = responseMap.value("date").toUInt();

        if (!responseMap.value("attachments").isNull())
        {
//            if (!responseMap.value("attachments").value().)
//          qDebug() << responseMap.value("attachments");
            m_attachments = new QList<Vkattach*>();
            QVariantList attachmentsList = responseMap.value("attachments").value<QVariantList>();
//            QString photoURL, photoURL_large, _url;
//            int _attachmentType;
            foreach (const QVariant v2, attachmentsList)
            {
                m_attachments->append(new Vkattach(m_id, false, v2, this) );
            }
//            emit message_parsed(responseMap.value("id").toString());
        }
    }

    const QList<Vkattach*>* kartun_vkchat::VKWallComment::getAttachments()
    {
        return m_attachments;
    }

    int kartun_vkchat::VKWallComment::getFromId() const
    {
        return m_fromId;
    }

    VKGeo* kartun_vkchat::VKWallComment::getGeo() const
    {
        return m_geo;
    }

    uint kartun_vkchat::VKWallComment::getId() const
    {
        return m_id;
    }

    uint kartun_vkchat::VKWallComment::getReplyToComment() const
    {
        return m_replyToComment;
    }

    int kartun_vkchat::VKWallComment::getReplyToUser() const
    {
        return m_replyToUser;
    }

    const QString& kartun_vkchat::VKWallComment::getText() const
    {
        return m_text;
    }

    uint kartun_vkchat::VKWallComment::getTimestamp() const
    {
        return m_timestamp;
    }


} /* namespace kartun_vkchat */
