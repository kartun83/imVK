/*
 * MessageDataModel.cpp
 *
 *  Created on: Nov 26, 2015
 *      Author: kartun
 */

#include "MessageDataModel.h"

namespace kartun_vkchat
    {

        MessageDataModel::MessageDataModel(QMap<uint, Vkmessage*> &inContainer, QObject* parent) : bb::cascades::DataModel(parent)
        {
            // TODO Auto-generated constructor stub
            m_items = new QMap<uint, Vkmessage*>( inContainer);
            m_indexPathCache = new QHash<int, uint>();
            // TODO :: Check for better strategy
            m_indexPathCache->reserve(300); // Reserve space for 300 messages
            int i=0;
            qDebug() << "Received container:" << inContainer.count();
            foreach (Vkmessage* message, inContainer)
            {
                m_indexPathCache->insert(i++, message->getMessageID());
                // Connect to attachments urlChanged signal if it's not local URL
                int l_count = message->getAttachmentCount();
                if (l_count > 0)
                {
                    for (int i=0;i<l_count;++i)
                    {
                        Vkattach* l_attach = message->getAttachment(i);
                        if (!l_attach->isLocal())
                        {
                            bool connect_result;
                            connect_result = connect(l_attach, SIGNAL(urlChanged(QString)), this, SLOT(onUrlChanged(QString)));
                            Q_ASSERT(connect_result);
                            Q_UNUSED(connect_result);
                        }
                    }
                }
            }
        }

        // TODO :: Make proper
        void MessageDataModel::insertItem( Vkmessage* &newMessage )
        {
            // TODO :: Check for duplicates
            Vkmessage* l_message = newMessage;

            // TODO :: Make proper with random inserts
//            QMap<uint, Vkmessage*>::const_iterator match = m_items->lowerBound(l_message->getMessageID());
//            if (match == m_items->begin())
//            {
//
//            }
//            else
//            {
//
//            }
            if (m_items->value(l_message->getMessageID())) { return; }
            m_indexPathCache->insert(m_indexPathCache->count(), l_message->getMessageID());
            qDebug() << "Inserting:" << l_message->getMessageID();
            m_items->insert(l_message->getMessageID(), l_message);
            QVariantList indexPath;
            // TODO :: Make cache for reverse lookup
            indexPath << m_indexPathCache->count()-1;
            bool connect_result;
            // Read state might change
            if (l_message->getReadState() != Vkmessage::read)
            {
                connect_result = connect(l_message, SIGNAL(readStateChanged(Vkmessage::ReadState )),
                                         this, SLOT(onReadStateChanged(Vkmessage::ReadState)) );
                Q_ASSERT(connect_result);
                Q_UNUSED(connect_result);
            }
            // Hook to attachments signals urlUpdated if it's not local files already
            int l_count = l_message->getAttachmentCount();
            if (l_count > 0)
            {
                for (int i=0;i<l_count;++i)
                {
                    Vkattach* l_attach = l_message->getAttachment(i);
                    if (!l_attach->isLocal())
                    {
                        connect_result = connect(l_attach, SIGNAL(urlChanged(QString)), this, SLOT(onUrlChanged(QString)));
                        Q_ASSERT(connect_result);
                        Q_UNUSED(connect_result);
                    }
                }
            }
            emit itemAdded(indexPath);
        }

        MessageDataModel::MessageDataModel(QObject* parent) : bb::cascades::DataModel(parent)
        {
            m_items = new QMap<uint, Vkmessage*>();
            m_indexPathCache = new QHash<int, uint>();
            // TODO :: Check for better strategy
            m_indexPathCache->reserve(300); // Reserve space for 300 messages
            qCritical() << "Shit!";
        }

        int MessageDataModel::childCount(const QVariantList& indexPath)
        {
//            qDebug() << "On child count:" << m_items->count();
            const int level = indexPath.size();
            int result = 0;
            int header;

            // We allways have message, and it's toplevel
            switch (level) {
                case 0: result = m_items->count(); break;
                case 1: header = indexPath[0].toInt();
                        if (header < 0) { result = 0; } else {
                        // We return message on level 1
//                        Q_ASSERT(m_items);
//                        qDebug() << "Items in index path cache:" << m_indexPathCache->count();
//                        Q_ASSERT(m_indexPathCache->value(header));
//                        Q_ASSERT(m_items->value(m_indexPathCache->value(header)));
                        result = m_items->value(m_indexPathCache->value(header))->getAttachmentCount() + 1; }
                        break;
                default:
                    break;
            }

            // The number of child items for 2nd level items is requested -> always 0
//            qDebug() << "Childcount for" << header << " = "<< result;
            return result;
        }

        bool MessageDataModel::hasChildren(const QVariantList& indexPath)
        {
            // Performance is not an issue with this data model.
            // So just let childCount tell us if we have children.
            // TODO :: Possibly need another cache for child count
            return childCount(indexPath) > 0;
        }

        QVariant MessageDataModel::data(const QVariantList& indexPath)
        {
            QVariant value;

                   if (indexPath.size() == 1) {
                       // Header requested
                       value.setValue( m_items->value(m_indexPathCache->value(indexPath[0].toInt())) );

                   }

                   if (indexPath.size() == 2) { // 2nd-level item requested
                       const int header = indexPath[0].toInt();
                       const int childItem = indexPath[1].toInt();
                       // At level 0 return message itself
                       if (childItem == 0)
                       {
                           value.setValue( m_items->value(m_indexPathCache->value(header)) );
                       }
                       else
                       {
                           // As 0 we returned message, so we fetch -1 of the index path
                           value.setValue(m_items->value(m_indexPathCache->value(header))->getAttachment(childItem-1));
//                           value.setValue("Nothing here");
                       }
//                       qDebug() << "Data for " << indexPath << " is " << value;
                   }

                   return QVariant(value);
        }

        QString MessageDataModel::itemType(const QVariantList& indexPath)
        {
            switch (indexPath.size()) {
                case 0:
                    return QString();
                    break;
                case 1:
                    return QLatin1String("header");
                    break;
                case 2:
                    return QLatin1String("item");
                                            break;
                default:
                    return QLatin1String("item");
                    break;
            }
        }

    void MessageDataModel::onUrlChanged(QString newUrl)
    {
        Q_UNUSED(newUrl);
        Vkattach* l_sender = qobject_cast<Vkattach*>( QObject::sender() );
        int message_id = l_sender->getMessageID();
        uint self_id   = l_sender->getIdx() + 1;    // At 0 there is a message, so add 1 to reflect actual index in model
        // TODO :: May be go for second cache for reverse lookup
        // Slow, but avoids to maintaining second cache
        int idx_path = m_indexPathCache->key(message_id);
        QVariantList indexPath;
        // TODO :: Make cache for reverse lookup
        indexPath << idx_path << self_id;
        // As we downloaded media connection isn't required anymore
        bool disconnect_result;
        disconnect_result = disconnect(l_sender, SIGNAL(urlChanged(QString)), this, SLOT(onUrlChanged(QString)));
        Q_ASSERT(disconnect_result);

        emit itemUpdated(indexPath);
    }

    void MessageDataModel::onReadStateChanged(Vkmessage::ReadState newReadState)
    {
        Vkmessage* l_sender = qobject_cast<Vkmessage*>( QObject::sender() );
        int message_id = l_sender->getMessageID();
        int idx_path = m_indexPathCache->key(message_id);
        QVariantList indexPath;
        // TODO :: Make cache for reverse lookup
        indexPath << idx_path;
        // As message is read connection isn't required anymore
        if (newReadState == Vkmessage::read)
        {
            bool disconnect_result;
            disconnect_result = disconnect(l_sender, SIGNAL(urlChanged(QString)), this, SLOT(onUrlChanged(QString)));
            Q_ASSERT(disconnect_result);
        }

        emit itemUpdated(indexPath);
    }

    bool MessageDataModel::isEmpty()
    {
        return m_items->empty();
    }

    void MessageDataModel::clear()
    {
        m_items->clear();
        m_indexPathCache->clear();
    }


} /* namespace vkChatTest */


