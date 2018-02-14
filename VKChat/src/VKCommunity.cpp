/*
 * VKCommunity.cpp
 *
 *  Created on: Dec 8, 2015
 *      Author: kartun
 */

#include <src/VKCommunity.hpp>

namespace kartun_vkchat
{
//    VKCommunity::Initializer::Initializer()
//    {
//        VKCommunity::m_page_types.insert("group", 1);
//        VKCommunity::m_page_types.insert("page",  2);
//        VKCommunity::m_page_types.insert("event", 3);
//    }

    VKCommunity::VKCommunity(QObject* parent) : QObject(parent)
    {
        // TODO Auto-generated constructor stub
        m_page_types.insert("group", 1);
        m_page_types.insert("page",  2);
        m_page_types.insert("event", 3);
        ////
    }

    VKCommunity::~VKCommunity()
    {
        // TODO Auto-generated destructor stub
    }

    VKCommunity::VKCommunity(const QVariant& json, QObject* parent) : QObject(parent)
    {
        m_page_types.insert("group", 1);
        m_page_types.insert("page",  2);
        m_page_types.insert("event", 3);

        QMap<QString, QVariant> attachmentsMap = json.toMap();

        m_id = attachmentsMap.value("id").toUInt();
        m_name = attachmentsMap.value("name").toString();
        m_screen_name = attachmentsMap.value("screen_name").toString();
        m_is_closed = (CommunityAccessLevel)attachmentsMap.value("is_closed").toInt();
        m_isAdmin   = attachmentsMap.value("id").toBool();
        m_photo_100 = attachmentsMap.value("photo_100").toString();
        m_photo_200 = attachmentsMap.value("photo_200").toString();
        m_type      = (CommunityType)m_page_types.value(attachmentsMap.value("type").toString());
        m_status    = attachmentsMap.value("status").toString();
//                AdminLevel  m_adminLevel;
//                bool        m_isMember;
//                int         m_invitedBy = attachmentsMap.value("id").toUInt();
//                bool        m_hasPhoto;

//                QString     m_description;
//                QString     m_status;
//                uint        m_members_count;
//                bool        m_can_post;
//                bool        m_can_see_all_posts;
//                bool        m_can_upload_doc;
//                bool        m_can_upload_video;
//                bool        m_can_create_topic;
//                MainSection m_main_section;
//                MemberStatus m_member_status;
    }

    VKCommunity::CommunityType VKCommunity::getType() const
    {
        return m_type;
    }

    VKCommunity::AdminLevel kartun_vkchat::VKCommunity::getAdminLevel() const
    {
        return m_adminLevel;
    }

    bool kartun_vkchat::VKCommunity::isCanCreateTopic() const
    {
        return m_can_create_topic;
    }

    bool kartun_vkchat::VKCommunity::isCanPost() const
    {
        return m_can_post;
    }

    bool kartun_vkchat::VKCommunity::isCanSeeAllPosts() const
    {
        return m_can_see_all_posts;
    }

    bool kartun_vkchat::VKCommunity::isCanUploadDoc() const
    {
        return m_can_upload_doc;
    }

    bool kartun_vkchat::VKCommunity::isCanUploadVideo() const
    {
        return m_can_upload_video;
    }

    const QString& kartun_vkchat::VKCommunity::getDescription() const
    {
        return m_description;
    }

    bool kartun_vkchat::VKCommunity::isHasPhoto() const
    {
        return m_hasPhoto;
    }

    uint kartun_vkchat::VKCommunity::getId() const
    {
        return m_id;
    }

    int kartun_vkchat::VKCommunity::getInvitedBy() const
    {
        return m_invitedBy;
    }

    kartun_vkchat::VKCommunity::CommunityAccessLevel VKCommunity::isClosed() const
    {
        return m_is_closed;
    }

    bool kartun_vkchat::VKCommunity::isAdmin() const
    {
        return m_isAdmin;
    }

    bool kartun_vkchat::VKCommunity::isMember() const
    {
        return m_isMember;
    }

    void VKCommunity::setStatus(const QString& status)
    {
        m_status = status;
        emit statusChanged(m_status);
    }

    void VKCommunity::setPhoto200(const QString& photo200)
    {
        m_photo_200 = photo200;
        emit avatarChanged(m_photo_200);
    }

    uint kartun_vkchat::VKCommunity::getMembersCount() const
    {
        return m_members_count;
    }

    const QString& kartun_vkchat::VKCommunity::getName() const
    {
        return m_name;
    }

    const QString& kartun_vkchat::VKCommunity::getPhoto100() const
    {
        return m_photo_100;
    }

    const QString& kartun_vkchat::VKCommunity::getPhoto200() const
    {
        return m_photo_200;
    }

    const QString& kartun_vkchat::VKCommunity::getScreenName() const
    {
        return m_screen_name;
    }

    VKCommunity::MemberStatus kartun_vkchat::VKCommunity::getMemberStatus() const
    {
        return m_member_status;
    }

    VKCommunity::MainSection kartun_vkchat::VKCommunity::getMainSection() const
    {
        return m_main_section;
    }

    const QString& kartun_vkchat::VKCommunity::getStatus() const
    {
        return m_status;
    }

} /* namespace kartun_vkchat */
