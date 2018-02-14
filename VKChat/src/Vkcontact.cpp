/*
 * Vkcontact.cpp
 *
 *  Created on: Aug 23, 2015
 *      Author: Kartun
 */

#include "Vkcontact.hpp"
#include <QtCore/QDebug>

namespace kartun_vkchat
{
    // Default constructor, required for QMap
    Vkcontact::Vkcontact(QObject* parent) : QObject(parent)
    {
        this->_id = 0;
        this->_first_name = "";
        this->_last_name  = "";
        this->_photo_100  = "";
        this->_online     = false;
        this->_status     = "";
        this->_deactivated = 0;
    }

    Vkcontact::Vkcontact(uint contactID, QString firstName, QString lastName, QString photo100,
            bool online, QString status, bool deactivated, QObject *parent) : QObject(parent)
    {
        // TODO Auto-generated constructor stub
        this->_id = contactID;
        this->_first_name = firstName;
        this->_last_name  = lastName;
        this->_photo_100  = photo100;
        this->_online     = online;
        this->_status     = status;
        this->_deactivated = deactivated;
    }
    kartun_vkchat::Vkcontact::Vkcontact(const QVariant& json, QObject* parent) : QObject(parent)
    {
        QMap<QString, QVariant> responseMap = json.toMap();

        qDebug() << "Creating USER from json:" << responseMap;
        this->_id = responseMap.value("user_id").toUInt();
        if (_id == 0) { this->_id = responseMap.value("id").toUInt(); }
        this->_first_name = responseMap.value("first_name").toString();
        this->_last_name  = responseMap.value("last_name").toString();
        this->_photo_100  = responseMap.value("photo_100").toString();
        this->_online     = responseMap.value("online").toBool();
        this->_status     = responseMap.value("status").toString();
        // TODO :: Finish
//        if (responseMap.count("deactivated") > 0)
//        {
//            //QString _reason = map.value("deactivated").toString();
//            //int _reason = deactivatedReason.value(responseMap.value("deactivated").toString());
//            this->_deactivated = _reason;
//        }
        this->_deactivated = false;//responseMap.value("photo_100").toString();;
    }


    Vkcontact::~Vkcontact()
    {
        // TODO Auto-generated destructor stub
    }

    QString Vkcontact::getFirstName() const
    {
        return this->_first_name;
    }

    QString Vkcontact::getLastName() const
    {
        return this->_last_name;
    }

    QString Vkcontact::getSmallAvatar() const
    {
        return this->_photo_100;
    }

    void Vkcontact::setSmallAvatar(QString newAvatar)
    {
        this->_photo_100 = newAvatar;
        emit smallAvatarChanged(newAvatar);
    }

    uint    Vkcontact::getContactID() const
    {
        return _id;
    }

    QString Vkcontact::getStatus() const
    {
        return this->_status;
    }

    bool Vkcontact::getOnline() const
    {
        return this->_online;
    }

    void Vkcontact::setOnline(bool online)
    {
        this->_online = online;
        emit onlineChanged(online);
    }

    uint Vkcontact::getDeactivated() const
    {
        return this->_deactivated;
    }
    QString Vkcontact::getContactID_str() const
    {
        return QString::number(this->_id);
    }

    void Vkcontact::setDeactivated(uint reason)
    {
        Q_ASSERT_X(reason <=2, "Vkcontact::setDeactivated", "Invalid value");
        _deactivated = reason;
    }

} /* namespace kartun_vkchat */


