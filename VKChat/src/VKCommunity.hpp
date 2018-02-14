/*
 * VKCommunity.hpp
 *
 *  Created on: Dec 8, 2015
 *      Author: kartun
 */

#ifndef VKCOMMUNITY_HPP_
#define VKCOMMUNITY_HPP_
#include <QtCore/QObject>

namespace kartun_vkchat
{

    class VKCommunity: public QObject
    {
        Q_OBJECT
        Q_ENUMS(CommunityAccessLevel);
        Q_ENUMS(AdminLevel);
        Q_ENUMS(MemberStatus);
        Q_ENUMS(MainSection);
        Q_ENUMS(CommunityType);
        // Properties
        Q_PROPERTY(uint id  READ getId)
        Q_PROPERTY(QString name READ getName)
        Q_PROPERTY(QString screenName READ getScreenName)
        Q_PROPERTY(QString status READ getStatus WRITE setStatus NOTIFY statusChanged)
        Q_PROPERTY(bool closed READ isClosed)
        Q_PROPERTY(QString photo_200 READ getPhoto200 WRITE setPhoto200 NOTIFY avatarChanged)
        Q_PROPERTY(VKCommunity::CommunityType communityType READ getType)
        // Inner friend utility class to initialize whatever you need

//        class Initializer
//        {
//        public:
//            Initializer();
//        };

//        friend class Initializer;
    public:
        enum CommunityAccessLevel { Open = 0, Closed = 1, Private = 2 };
        enum AdminLevel { Moderator = 1, Editor = 2, Administrator = 3 };
        enum MemberStatus { NotJoined = 0, Joined = 1, Unsure = 2, Declined = 3, Applicant = 4, Invited = 5 };
        enum MainSection  { None = 0, Photo = 1, Discussion = 2, Audio = 3, Video = 4, Goods = 5};
        enum CommunityType { Group = 1, Page = 2 , Event = 3  };
        VKCommunity(QObject* parent = 0);
        VKCommunity(const QVariant &json, QObject* parent = 0);
        virtual ~VKCommunity();

    bool isCanCreateTopic() const;
    bool isCanPost() const;
    bool isCanSeeAllPosts() const;
    bool isCanUploadDoc() const;
    bool isCanUploadVideo() const;
    const QString& getDescription() const;
    bool isHasPhoto() const;
    uint getId() const;
    int  getInvitedBy() const;
    CommunityAccessLevel isClosed() const;
    bool isAdmin() const;
    bool isMember() const;
    uint getMembersCount() const;
    const QString& getName() const;
    const QString& getPhoto100() const;
    const QString& getPhoto200() const;
    const QString& getScreenName() const;
    const QString& getStatus() const;
    MemberStatus getMemberStatus() const;
    MainSection  getMainSection() const;
    AdminLevel   getAdminLevel() const;
    CommunityType getType() const;
    void setPhoto200(const QString& photo200);
    void setStatus(const QString& status);

        void setPhoto100(const QString& photo100)
        {
            m_photo_100 = photo100;
        }

    Q_SIGNALS:
        void statusChanged(QString newStatus);
        void avatarChanged(QString newAvatar);

    private:
        uint m_id;
        QString m_name;
        QString m_screen_name;
        CommunityAccessLevel m_is_closed;
        bool        m_isAdmin;
        AdminLevel  m_adminLevel;
        bool        m_isMember;
        int         m_invitedBy;
        bool        m_hasPhoto;
        QString     m_photo_100;
        QString     m_photo_200;
        QString     m_description;
        QString     m_status;
        uint        m_members_count;
        bool        m_can_post;
        bool        m_can_see_all_posts;
        bool        m_can_upload_doc;
        bool        m_can_upload_video;
        bool        m_can_create_topic;
        MainSection m_main_section;
        MemberStatus    m_member_status;
        CommunityType   m_type;
        QMap<QString, uint> m_page_types;
    protected:
//        static Initializer initializer;
    };

} /* namespace kartun_vkchat */

Q_DECLARE_METATYPE( kartun_vkchat::VKCommunity *);
#endif /* VKCOMMUNITY_HPP_ */
