/*
 * VKGeo.hpp
 *
 *  Created on: Dec 23, 2015
 *      Author: kartun
 */

#ifndef VKGEO_HPP_
#define VKGEO_HPP_

#include <QtCore/QObject>
#include <QtCore/QDebug>

namespace kartun_vkchat
{

    class VKGeo: public QObject
    {
        Q_OBJECT

        // Properties
        Q_PROPERTY( QString lattitude READ getLattitude CONSTANT )
        Q_PROPERTY( QString longtitude READ getLongtitude CONSTANT )
    public:
        VKGeo(const QVariant &json, QObject *parent = 0);
        VKGeo(QObject *parent = 0);
        virtual ~VKGeo()
        {
            // TODO Auto-generated destructor stub
        }

        const QString& getCity() const;
        const QString& getCountry() const;
        const QString& getLattitude() const;
        const QString& getLongtitude() const;
        const QString& getTitle() const;
        const QString& getType() const;

    private:
        QString m_type;
        QString m_lattitude;
        QString m_longtitude;
        QString m_country;
        QString m_city;
        QString m_title;
    };

} /* namespace kartun_vkchat */

Q_DECLARE_METATYPE( kartun_vkchat::VKGeo *);

#endif /* VKGEO_HPP_ */
