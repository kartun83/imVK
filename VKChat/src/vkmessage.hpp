/*
 * vkmessage.h
 *
 *  Created on: Aug 20, 2015
 *      Author: Kartun
 */

#ifndef VKMESSAGE_H_
#define VKMESSAGE_H_

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include "vkauth.hpp"
#include "VKGeo.hpp"
#include "VKmessageattachment.hpp"

//using namespace bb::cascades;

namespace kartun_vkchat
{

    class Vkmessage: public QObject
    {
        Q_OBJECT

        // Properties
        Q_PROPERTY(int message_id READ getMessageID CONSTANT);
        Q_PROPERTY(int contentType READ getContentType CONSTANT);
        Q_PROPERTY(uint from_id READ getFromID CONSTANT);
        Q_PROPERTY(uint user_id READ getUserID CONSTANT);
        Q_PROPERTY(uint date READ getTimestamp CONSTANT);
        Q_PROPERTY(bool out READ getOut CONSTANT);
        Q_PROPERTY(Vkmessage::ReadState read_state READ getReadState WRITE setReadState NOTIFY readStateChanged);
        Q_PROPERTY(QString subject READ getSubject CONSTANT);
        Q_PROPERTY(QString body READ getText WRITE setText);
        Q_PROPERTY(int messageContentType READ getMessageContentType CONSTANT);
        Q_PROPERTY(QString first_name READ getFirstName CONSTANT);
        Q_PROPERTY(QString last_name READ getLastName CONSTANT)
        Q_PROPERTY(bool hasAttachment READ getHasAttachment CONSTANT);
        Q_PROPERTY(int lastAttachmentType READ getLastAttachmentType CONSTANT);
        Q_PROPERTY(bool has_geo READ isHasGeo CONSTANT);
        Q_PROPERTY(VKGeo* geo READ getGeo CONSTANT);
//        Q_PROPERTY(uint recepientID READ getRecepientID CONSTANT);

        Q_ENUMS(kartun_vkchat::ReadState)
    public:
        enum ReadState {
                sending = -1,
                notRead = 0,
                read    = 1,
            };
        // TODO :: Check if it could be moved into single header
//        typedef enum {
//                    _attachment_unknown = 0,
//                    _attachment_photo = 1,
//                    _attachment_video = 2,
//                    _attachment_audio = 3,
//                    _attachment_doc   = 4,
//                    _attachment_wall  = 5,
//
//                } attachmentType;
//        struct gst_attachment{
//            int type;
//            QString _url;
//        };
        explicit Vkmessage(QObject *parent = 0) : QObject(parent) { }
        Vkmessage(const Vkmessage &other, QObject *parent = 0);
        Vkmessage(int message_id, uint from_id, uint user_id, uint timestamp, bool out, Vkmessage::ReadState read_state, QString subject,
                   QString text, int messageContentType, QObject *parent = 0);
        Vkmessage(vk_api::new_message_struct newMessage, const uint &myID, const QVariant &attach = QVariant::Invalid, QObject *parent = 0);
        Vkmessage(const QVariant &json, const uint &myID, QObject *parent);
        virtual ~Vkmessage();
        // Accessor methods
        int  getMessageID() const;
        uint getFromID() const;
        uint getUserID() const;
//        uint getRecepientID() const;
        uint getTimestamp() const;
        bool getOut() const;
        bool getHasAttachment() const;
        Vkmessage::ReadState getReadState() const;
        void setReadState(Vkmessage::ReadState readState);
        QString getSubject() const;
        QString getText() const;
        void setText(QString newText);
        void appendText(QString addText);
        int getMessageContentType() const;
        QString getFirstName() const;
        QString getLastName() const;
//        QString getFullName() const;
        void addAttachment(uint type, QString uri) const;
        void addAttachment(Vkattach* newAttachment) const;
        void addAttachments(const QList<Vkattach*>& attachments);
        Vkattach* getAttachment(int index);
        int getAttachmentCount();
        int getLastAttachmentType();
        QList<Vkattach*>* getAttachments() const;
        void setParent();
        bool isHasGeo() const;
        VKGeo* getGeo() const;
    Q_SIGNALS:
        void readStateChanged(Vkmessage::ReadState newState);
        void attachmentChanged(int attachmentID);
    public Q_SLOTS:
        void onUrlChanged(QString newUrl);


    private:
//        bb::cascades::GroupDataModel* getAttachmentsModel();
//        bb::cascades::GroupDataModel* _myAttahcments;
        int getContentType();
//        template<class T> friend class QList;
        // $message_id,$flags,$from_id,$timestamp,$subject,$text,$attachments
        int     _message_id;
        uint    _from_id;    // message author
        uint    _user_id;    // ??
//        uint    _recepientID;   // Must not be own ID
        uint    _timestamp;
        bool    _out;        // 0 - received, 1 -sent
//        bool    _has_geo;    // Has geo information
        Vkmessage::ReadState    _read_state; // -1 sending, 0 - not readed, 1 - read
        QList<Vkattach*>*  _attachments;
        QString _subject;
        QString _text;
        int     _messageContentType;
        VKGeo*  _geo;
        // attachments
        // fwd_messages
    };

} /* namespace kartun_vkchat */
Q_DECLARE_METATYPE( kartun_vkchat::Vkmessage *);

#endif /* VKMESSAGE_H_ */
