/*
 * vkmessage.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: Kartun
 */

#include "vkmessage.hpp"

namespace kartun_vkchat
{

    Vkmessage::Vkmessage(int message_id, uint from_id, uint user_id, uint timestamp, bool out, Vkmessage::ReadState read_state, QString subject,
            QString text, int messageContentType, QObject *parent) : QObject(parent)
    {
        // TODO Auto-generated constructor stub
        _geo = NULL;
        _message_id = message_id;
        _from_id    = from_id;    // message author
        _user_id    = user_id;    // message recipient
        _timestamp  = timestamp;
        _out        = out;        // 0 - received, 1 -sent
        _read_state = read_state; // 0 - not readed, 1 - read
        _subject    = subject;
        _text       = text;
        _messageContentType = messageContentType;
        _attachments = new QList<Vkattach*>();
//        _user_id    = user_id;
    }

    kartun_vkchat::Vkmessage::Vkmessage(const Vkmessage& other, QObject* parent) : QObject(parent)
    {
        _geo = NULL;
        _message_id = other._message_id;
        _from_id    = other._from_id;    // message author
        _user_id    = other._user_id;    // message recepient
        _timestamp  = other._timestamp;
        _out        = other._out;        // 0 - received, 1 -sent
        _read_state = other._read_state; // 0 - not readed, 1 - read
        _subject    = other._subject;
        _text       = other._text;
        _messageContentType = other._messageContentType;
        _attachments = new QList<Vkattach*>();
    }

    Vkmessage::Vkmessage(vk_api::new_message_struct newMessage, const uint &myID, const QVariant &attach, QObject *parent) : QObject(parent)
    {
        _message_id = newMessage.message_id;
//        _from_id    = newMessage.from_id;    // message author
//        _user_id    = newMessage.user_id;    // message recipient
        _timestamp  = newMessage.timestamp;
        _out        = newMessage.out;        // 0 - received, 1 -sent
        _read_state = (Vkmessage::ReadState)newMessage.read;       // 0 - not readed, 1 - read
        _subject    = newMessage.subject;
        _text       = newMessage.text;
        _messageContentType = 100;  // TODO :: Make proper :)
        _attachments = new QList<Vkattach*>();
        _geo = NULL;
        if (_out) { _from_id = myID; _user_id = newMessage.user_id;} else { _user_id = myID; _from_id = newMessage.from_id; }
        if (_from_id == 0 || _user_id == 0 || _from_id == _user_id) { qCritical() << "UserID is invalid"; }
        // Parse attachments, if any
        if (!attach.isNull() && attach.isValid())
        {
            qDebug() << "Parsing attachment:";
            qDebug() << attach;

            QVariantMap singleResponse = attach.value<QVariantMap>();
            qDebug() << singleResponse.value("emoji");
            // TODO :: Fix crash on geo !!!
            if (!singleResponse.isEmpty() && singleResponse.value("emoji").toString() != "1")
            {
//            QString field1 = singleResponse.at(0);
//            QString field2 = singleResponse.at(0);
//            QString field3 = singleResponse.at(0);
                QVariantList attachmentList;
                foreach(const QVariant& v, singleResponse)
                {
                     attachmentList.append(v);
                }
                qDebug() << "passing parsed list:" << attachmentList;
                _attachments->append(new Vkattach(_message_id, _out, attachmentList, this) );
            }
            else
            {
                qDebug() << "Empty attachment:" << singleResponse;
            }
        }
    }

    Vkmessage::Vkmessage(const QVariant &json, const uint &myID, QObject* parent) : QObject(parent)
    {
        _geo = NULL;
        QMap<QString, QVariant> responseMap = json.toMap();

        qDebug() << "Creating message from json" << responseMap;
        // From dialogs list we get "message" object
        QVariantMap message = responseMap.value("message").value<QVariantMap>();
        // In actual chat we get element "items" which would be unrolled on top of hierarchy
        if (message.isEmpty()) { message = responseMap; }
        _message_id = message.value("id").toInt();
        _from_id    = message.value("from_id").toUInt();
        _user_id    = message.value("user_id").toUInt();
        _out        = message.value("out").toBool();
        // For dialogs for some reason there are different keys
        uint l_uid = _user_id;
//        if (_message_id == 0) { _message_id = responseMap.value("mid").toInt(); }
//        if (_user_id == 0) { l_uid = responseMap.value("uid").toUInt(); }
        _timestamp  = message.value("date").toUInt();
        if (_out) { _from_id = myID; _user_id = l_uid;} else { _user_id = myID; _from_id = l_uid; }
//        _read_state = (Vkmessage::ReadState)responseMap.value("read_state").toInt();       // 0 - not readed, 1 - read
        _read_state = (Vkmessage::ReadState)responseMap.value("read_state").toInt();       // 0 - not readed, 1 - read
        _subject    = message.value("title").toString();
        _text       = message.value("body").toString();
        if (!message.value("geo").isNull())
        {
            this->_geo = new VKGeo(message.value("geo"), this);
            if (_text.isEmpty())
            {
                // Override text with description
                _text = QString(QObject::tr("Map link")+" to %1").arg(_geo->getTitle());
            }
        };
        _messageContentType = 100;

        // TODO :: Add some variables to avoid crashing and minimize memory usage
        _attachments = new QList<Vkattach*>();
        if (!message.value("attachments").isNull())
        {
//            if (!responseMap.value("attachments").value().)
//          qDebug() << responseMap.value("attachments");
            QVariantList attachmentsList = message.value("attachments").value<QVariantList>();
//            QString photoURL, photoURL_large, _url;
//            int _attachmentType;
            foreach (const QVariant v2, attachmentsList)
            {
                _attachments->append(new Vkattach(_message_id, _out, v2, this) );
            }
//            emit message_parsed(responseMap.value("id").toString());
        }


    }

    Vkmessage::~Vkmessage()
    {
        // TODO Auto-generated destructor stub
    }

    // Accessors
    int  Vkmessage::getMessageID() const
    {
        return this->_message_id;
    }

    uint  Vkmessage::getFromID() const
    {
        return this->_from_id;
    }

    uint  Vkmessage::getUserID() const
    {
        return this->_user_id;
    }

    uint Vkmessage::getTimestamp() const
    {
        return this->_timestamp;
    }

    bool Vkmessage::getOut() const
    {
        return this->_out;
    }

    Vkmessage::ReadState Vkmessage::getReadState() const
    {
        return this->_read_state;
    }

    void Vkmessage::setReadState(Vkmessage::ReadState readState)
    {
        this->_read_state  = readState;
        emit readStateChanged(this->_read_state);
    }

    QString Vkmessage::getSubject() const
    {
        return this->_subject;
    }

    QString Vkmessage::getText() const
    {
        //QString tmp = QString("%1, attachments%2").arg(this->_text, QString::number(_attachments->count()));
        //return tmp;
        return this->_text;
    }

    void Vkmessage::setText(QString newText)
    {
        this->_text = newText;
    }

    void Vkmessage::appendText(QString addText)
    {
        _text.append(addText);
    }

    int Vkmessage::getMessageContentType() const
    {
        return this->_messageContentType;
    }

    QString Vkmessage::getFirstName() const
    {
        return "First name";
    }

    QString Vkmessage::getLastName() const
    {
        return "Last name";
    }

    void Vkmessage::addAttachment(uint type, QString uri) const
    {
//        Vkattach*  newAttachment = new Vkattach(type, uri);
//        _attachments->append(newAttachment);
    }

    void kartun_vkchat::Vkmessage::addAttachment(Vkattach* newAttachment) const
    {
//        bool connect_result;
        _attachments->append(newAttachment);
//        if (!newAttachment->isLocal())
//        {
//            connect_result = connect(newAttachment, SIGNAL(urlChanged(QString)),
//                                     this, SLOT(onUrlChanged(QString)));
//            Q_ASSERT(connect_result);
//        }
    }

    void kartun_vkchat::Vkmessage::addAttachments(const QList<Vkattach*>& attachments)
    {
        Q_ASSERT(_attachments);
        if (!attachments.isEmpty()) {
            this->_attachments->append(attachments);
        }
    }
    int Vkmessage::getAttachmentCount()
    {
        int result = 0;
        qDebug() << "Attachments:" << _attachments;
        Q_ASSERT(_attachments);
        if (_attachments != NULL) { result = _attachments->count(); }
        return result;
    }

    Vkattach* Vkmessage::getAttachment(int index)
    {
        Q_ASSERT(_attachments);
        return _attachments->at(index);
    }

    QList<Vkattach*>* Vkmessage::getAttachments() const
    {
        Q_ASSERT(_attachments);
        return _attachments;
    }

    void Vkmessage::onUrlChanged(QString newUrl)
    {
        Q_UNUSED(newUrl);
        Vkattach* l_sender = qobject_cast<Vkattach*>( QObject::sender() );
        emit attachmentChanged( _attachments->indexOf(l_sender) );
    }

    int Vkmessage::getContentType()
    {
        return 0;
    }

//    uint Vkmessage::getRecepientID() const
//    {
//        return _recepientID;
//    }

    bool Vkmessage::getHasAttachment() const
    {
        return (bool)_attachments->count();
    }

    int kartun_vkchat::Vkmessage::getLastAttachmentType()
    {
        int result = -1;
        if (_attachments->count() > 0)
        {
            result = _attachments->last()->getMediaType();
        }
        return result;
    }

    bool Vkmessage::isHasGeo() const
    {
        bool result = _geo == NULL ? false : true;

        return result;
    }

    VKGeo* Vkmessage::getGeo() const
    {
        return this->_geo;
    }

} /* namespace kartun_vkchat */


