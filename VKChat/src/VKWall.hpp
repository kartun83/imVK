/*
 * VKWall.hpp
 *
 *  Created on: Dec 9, 2015
 *      Author: kartun
 */

#ifndef VKWALL_HPP_
#define VKWALL_HPP_

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include "VKmessageattachment.hpp"

namespace kartun_vkchat
{
    class VKWall: public QObject
    {
        Q_OBJECT

        Q_PROPERTY( bool canLike READ isCanLike NOTIFY canLikeChanged )
        Q_PROPERTY( bool canPin READ isCanPin NOTIFY canPinChanged )
        Q_PROPERTY( bool canPost READ isCanPost NOTIFY canPostChanged )
        Q_PROPERTY( bool canPublish READ isCanPublish NOTIFY canPublishChanged )
        Q_PROPERTY( uint date READ getDate  )
        Q_PROPERTY( bool friendsOnly READ isFriendsOnly  )
        Q_PROPERTY( int  fromID READ getFromId  )
        Q_PROPERTY( bool isPinned READ isIsPinned  )
        Q_PROPERTY( uint likesCount READ getLikesCount  )
        Q_PROPERTY( int  ownerID READ getOwnerId  )
        Q_PROPERTY( uint postID READ getPostId  )
        Q_PROPERTY( uint replyCount READ getReplyCount  )
        Q_PROPERTY( int  replyOwnerID READ getReplyOwnerId  )
        Q_PROPERTY( uint replyPostID READ getReplyPostId  )
        Q_PROPERTY( uint repostCount READ getRepostCount  )
        Q_PROPERTY( QString text READ getText  )
        Q_PROPERTY( bool userLike READ isUserLike  )
        Q_PROPERTY( bool userReposted READ isUserReposted  )

    public:
        VKWall(const QVariant &json, QObject* parent = 0);
        explicit VKWall(QObject* parent = 0) : QObject(parent) { };
        virtual ~VKWall();
        const QList<Vkattach*>* getAttachments() const;
        bool isCanLike() const;
        bool isCanPin() const;
        bool isCanPost() const;
        bool isCanPublish() const;
        uint getDate() const;
        bool isFriendsOnly() const;
        int  getFromId() const;
        bool isIsPinned() const;
        uint getLikesCount() const;
        int  getOwnerId() const;
        uint getPostId() const;
        uint getReplyCount() const;
        int  getReplyOwnerId() const;
        uint getReplyPostId() const;
        uint getRepostCount() const;
        const QString& getText() const;
        bool isUserLike() const;
        bool isUserReposted() const;

    Q_SIGNALS:
        void canLikeChanged(bool newCanLike);
        void canPinChanged(bool newCanPin);
        void canPostChanged(bool newCanPost);
        void canPublishChanged(bool newCanPublish);

    private:
        uint m_postId;
        int  m_ownerID;
        int  m_fromID;
        qlonglong m_date;
        QString   m_text;
        int       m_replyOwnerID;
        uint      m_replyPostID;
        bool      m_friendsOnly;
        uint      m_replyCount;
        bool      m_canPost;
        uint      m_likesCount;
        bool      m_userLike;
        bool      m_canLike;
        bool      m_canPublish;
        uint      m_repostCount;
        bool      m_userReposted;
        // postType
        // postSource
        QList<Vkattach*>* m_attachments;
        bool      m_canPin;
        bool      m_isPinned;
    };

} /* namespace kartun_vkchat */
Q_DECLARE_METATYPE( kartun_vkchat::VKWall *);
#endif /* VKWALL_HPP_ */
