/*
 * vkcontact.hpp
 *
 *  Created on: Aug 23, 2015
 *      Author: Kartun
 */

#ifndef VKCONTACT_HPP_
#define VKCONTACT_HPP_

#include <QtCore/QObject>

namespace kartun_vkchat
{

    class Vkcontact: public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString first_name READ getFirstName   CONSTANT);
        Q_PROPERTY(QString last_name  READ getLastName    CONSTANT);
        Q_PROPERTY(QString photo_100  READ getSmallAvatar WRITE setSmallAvatar NOTIFY smallAvatarChanged);
        Q_PROPERTY(uint    contactID READ getContactID   CONSTANT);
        Q_PROPERTY(bool    online READ getOnline WRITE setOnline NOTIFY onlineChanged);
        Q_PROPERTY(QString status READ getStatus CONSTANT);
        Q_PROPERTY(uint    deactivated READ getDeactivated WRITE setDeactivated);
        Q_PROPERTY(QString contactID_str READ getContactID_str CONSTANT);
        //Q_PROPERTY(QString READ get CONSTANT);
    public:

        Vkcontact(QObject *parent = 0);
        Vkcontact(uint contactID, QString firstName = "", QString lastName = "", QString photo100 = "",
                  bool online = false, QString status = "",bool deactivated = false, QObject *parent = 0);
        Vkcontact(const QVariant &json, QObject *parent = 0);
        virtual ~Vkcontact();

        // Accessors
        QString getFirstName() const;
        QString getLastName() const;
        QString getSmallAvatar() const;
        void    setSmallAvatar(QString newAvatar);
        uint    getContactID() const;
        QString getContactID_str() const;
        bool    getOnline() const;
        void    setOnline(bool online);
        QString getStatus() const;
        uint    getDeactivated() const;
        void    setDeactivated(uint reason);
    Q_SIGNALS:
        void smallAvatarChanged(QString newAvatar);
        void onlineChanged(bool onlineStatus);
    private:
        QString _first_name;
        QString _last_name;
        QString _status;
        QString _screen_name;
        uint    _id;
        bool    _online;    // 0 - offline, 1  - online
        QString _photo_100; // small avatar
        uint    _deactivated;   // 0 = active, 1 = banned, 2 = deleted
    };

} /* namespace kartun_vkchat */
Q_DECLARE_METATYPE( kartun_vkchat::Vkcontact *);

#endif /* VKCONTACT_HPP_ */
