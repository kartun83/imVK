/*
 * VKGeo.cpp
 *
 *  Created on: Dec 23, 2015
 *      Author: kartun
 */

#include <src/VKGeo.hpp>
#include <QtCore/QStringList>

namespace kartun_vkchat
{

    VKGeo::VKGeo(const QVariant &json, QObject *parent) : QObject(parent)
    {
        // TODO Auto-generated constructor stub
        QMap<QString, QVariant> responseMap = json.toMap();

        qDebug() << "Creating geo from JSON:" << responseMap;
                // From dialogs list we get "message" object
//        QVariantMap message = responseMap.value("message").value<QVariantMap>();
        QString l_coords = responseMap.value("coordinates").toString();
        if (!l_coords.isEmpty())
        {
            QStringList l_coords2 = l_coords.split(" ", QString::KeepEmptyParts, Qt::CaseInsensitive);
            m_lattitude = l_coords2.at(0);
            m_longtitude = l_coords2.at(1);
            QMap<QString, QVariant> placeMap = responseMap.value("place").toMap();
            qDebug() << "Place:" << placeMap;
            m_type = responseMap.value("type").toString();
            m_city = placeMap.value("city").toString();
            m_country = placeMap.value("country").toString();
            m_title   = placeMap.value("title").toString();
        }
    }

    const QString& VKGeo::getCity() const
    {
        return m_city;
    }

    const QString& VKGeo::getCountry() const
    {
        return m_country;
    }

    const QString& VKGeo::getLattitude() const
    {
        return m_lattitude;
    }

    const QString& VKGeo::getLongtitude() const
    {
        return m_longtitude;
    }

    const QString& VKGeo::getTitle() const
    {
        return m_title;
    }

    const QString& VKGeo::getType() const
    {
        return m_type;
    }

    kartun_vkchat::VKGeo::VKGeo(QObject* parent) : QObject(parent)
    {
        // Moscow Kremlin
        m_lattitude = "55.752032";
        m_longtitude = "37.616519";
        m_type = "point";
        m_country = "Russia";
        m_city    = "Moscow";
    }

} /* namespace kartun_vkchat */
