/*
 * VKWallComment.hpp
 *
 *  Created on: Dec 27, 2015
 *      Author: kartun
 */

#ifndef VKWALLCOMMENT_HPP_
#define VKWALLCOMMENT_HPP_

#include <QtCore/QObject>
#include <QtCore/QDebug>
//#include "vkauth.hpp"
#include "VKGeo.hpp"
#include "VKmessageattachment.hpp"

namespace kartun_vkchat
{

    class VKWallComment: public QObject
    {
        Q_OBJECT
        Q_PROPERTY( uint commentID READ getId CONSTANT  )
        Q_PROPERTY( int  fromID READ getFromId CONSTANT )
        Q_PROPERTY( uint timeStamp READ getTimestamp CONSTANT )
        Q_PROPERTY( QString text READ getText CONSTANT )
        Q_PROPERTY( int replyToUser READ getReplyToUser CONSTANT )
        Q_PROPERTY( uint replyToComment READ getReplyToComment CONSTANT )
        Q_PROPERTY( VKGeo* geo READ getGeo CONSTANT )

    public:
        explicit VKWallComment(QObject *parent = 0) : QObject(parent) { }
        VKWallComment(const QVariant &json, QObject *parent = 0);
        virtual ~VKWallComment()
        {
            // TODO Auto-generated destructor stub
        }

        const QList<Vkattach*>* getAttachments();
        int getFromId() const;
        VKGeo* getGeo() const ;
        uint getId() const;
        uint getReplyToComment() const;
        int getReplyToUser() const;
        const QString& getText() const;
        uint getTimestamp() const;

    private:
        uint    m_id;
        int     m_fromId;
        uint    m_timestamp;
        QString m_text;
        int     m_replyToUser;
        uint    m_replyToComment;
        VKGeo*  m_geo;
        QList<Vkattach*>*  m_attachments;
    };

} /* namespace kartun_vkchat */


Q_DECLARE_METATYPE( kartun_vkchat::VKWallComment *);
#endif /* VKWALLCOMMENT_HPP_ */
