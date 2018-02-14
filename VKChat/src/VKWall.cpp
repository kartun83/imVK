/*
 * VKWall.cpp
 *
 *  Created on: Dec 9, 2015
 *      Author: kartun
 */

#include <src/VKWall.hpp>

namespace kartun_vkchat
{

    VKWall::VKWall(const QVariant &json, QObject* parent) : QObject(parent)
    {
        m_attachments = new QList<Vkattach*>();
        QMap<QString, QVariant> responseMap = json.toMap();
//        qDebug() << "Parsing wall content:" << responseMap;

        m_postId = responseMap.value("id").toUInt();
        m_text   = responseMap.value("text").toString();
        m_date   = responseMap.value("date").toLongLong();
        //
        QVariantMap likes = responseMap.value("likes").value<QVariantMap>();
        qDebug() << "Likes section:" << likes;
        m_canLike = likes.value("can_like").toBool();
        m_userLike = likes.value("user_likes").toBool();
        m_canPublish = likes.value("can_publish").toBool();
        m_likesCount = likes.value("count").toUInt();

        QVariantMap reposts = responseMap.value("reposts").value<QVariantMap>();
        m_repostCount = reposts.value("count").toUInt();
        m_userReposted  = reposts.value("user_reposted").toBool();

        QVariantMap comments = responseMap.value("comments").value<QVariantMap>();
        m_replyCount = comments.value("count").toUInt();
        m_canPost    = comments.value("can_post").toBool();
//        "comments": {
//                        "count": 3,
//                        "can_post": 1
//                    }
    }

    const QList<Vkattach*>* VKWall::getAttachments() const
    {
        return m_attachments;
    }

    bool VKWall::isCanLike() const
    {
        return m_canLike;
    }

    bool VKWall::isCanPin() const
    {
        return m_canPin;
    }

    bool VKWall::isCanPost() const
    {
        return m_canPost;
    }

    bool VKWall::isCanPublish() const
    {
        return m_canPublish;
    }

    uint VKWall::getDate() const
    {
        return m_date;
    }

    bool VKWall::isFriendsOnly() const
    {
        return m_friendsOnly;
    }

    int VKWall::getFromId() const
    {
        return m_fromID;
    }

    bool VKWall::isIsPinned() const
    {
        return m_isPinned;
    }

    uint VKWall::getLikesCount() const
    {
        return m_likesCount;
    }

    int VKWall::getOwnerId() const
    {
        return m_ownerID;
    }

    uint VKWall::getPostId() const
    {
        return m_postId;
    }

    uint VKWall::getReplyCount() const
    {
        return m_replyCount;
    }

    int VKWall::getReplyOwnerId() const
    {
        return m_replyOwnerID;
    }

    uint VKWall::getReplyPostId() const
    {
        return m_replyPostID;
    }

    uint VKWall::getRepostCount() const
    {
        return m_repostCount;
    }

    const QString& VKWall::getText() const
    {
        return m_text;
    }

    bool VKWall::isUserLike() const
    {
        return m_userLike;
    }

    bool VKWall::isUserReposted() const
    {
        return m_userReposted;
    }

    VKWall::~VKWall()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace kartun_vkchat */
