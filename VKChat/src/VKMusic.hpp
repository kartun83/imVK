/*
 * VKMusic.hpp
 *
 *  Created on: Dec 1, 2015
 *      Author: kartun
 */

#ifndef VKMUSIC_HPP_
#define VKMUSIC_HPP_

#include <QtCore/QObject>
#include <QtCore/QDebug>

namespace kartun_vkchat
{

    class VKMusic: public QObject
    {
    public:
        bool isPlaying() const;
        void setPlaying(bool playing);

    private:
        Q_OBJECT

        Q_PROPERTY(uint id READ getId CONSTANT );
        Q_PROPERTY(uint owner_id READ getOwnerId CONSTANT);
        Q_PROPERTY(uint duration READ getDuration CONSTANT);
        Q_PROPERTY(uint date     READ getDate CONSTANT);
        Q_PROPERTY(QString artist READ getArtist CONSTANT);
        Q_PROPERTY(QString title  READ getTitle  CONSTANT);
        Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playingChanged)
        Q_PROPERTY(QString url    READ getUrl CONSTANT)
    public:
        VKMusic(const QVariant &json, QObject *parent = 0);
        virtual ~VKMusic()
        {
            // TODO Auto-generated destructor stub
        }

        const QString& getArtist() const;
        uint getDate() const;
        uint getDuration() const;
        uint getId() const;
        uint getOwnerId() const;
        const QString& getTitle() const;
        const QString& getUrl() const;
    Q_SIGNALS:
        void playingChanged(bool playingState);
    private:
        uint m_id;
        uint m_owner_id;
        QString m_artist;
        QString m_title;
        uint    m_duration;
        QString m_url;
        uint    m_date;
        bool    m_playing;
    };

} /* namespace kartun_vkchat */
Q_DECLARE_METATYPE( kartun_vkchat::VKMusic *);
#endif /* VKMUSIC_HPP_ */
