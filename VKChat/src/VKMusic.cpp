/*
 * VKMusic.cpp
 *
 *  Created on: Dec 1, 2015
 *      Author: kartun
 */

#include <src/VKMusic.hpp>

namespace kartun_vkchat
{
    VKMusic::VKMusic(const QVariant &json, QObject *parent) : QObject(parent)
    {
        // TODO Auto-generated constructor stub
        QMap<QString, QVariant> responseMap = json.toMap();

        m_id = responseMap.value("id").toUInt();
        m_owner_id = responseMap.value("owner_id").toUInt();
        m_duration = responseMap.value("duration").toUInt();
        m_artist = responseMap.value("artist").toString();
        m_title  = responseMap.value("title").toString();
        m_url    = responseMap.value("url").toString();
        m_date   = responseMap.value("date").toUInt();
    }

    const QString& VKMusic::getArtist() const
    {
        return m_artist;
    }

    uint VKMusic::getDate() const
    {
        return m_date;
    }

    uint VKMusic::getDuration() const
    {
        return m_duration;
    }

    uint VKMusic::getId() const
    {
        return m_id;
    }

    uint VKMusic::getOwnerId() const
    {
        return m_owner_id;
    }

    const QString& VKMusic::getTitle() const
    {
        return m_title;
    }

    const QString& VKMusic::getUrl() const
    {
        return m_url;
    }

    bool kartun_vkchat::VKMusic::isPlaying() const
    {
        return m_playing;
    }

    void kartun_vkchat::VKMusic::setPlaying(bool playing)
    {
        m_playing = playing;
        emit playingChanged(m_playing);
    }

} /* namespace kartun_vkchat */
