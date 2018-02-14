/*
 * MessageDataModel.h
 *
 *  Created on: Nov 26, 2015
 *      Author: kartun
 */

#ifndef MESSAGEDATAMODEL_H_
#define MESSAGEDATAMODEL_H_
#include <bb/cascades/DataModel>
#include "../vkmessage.hpp"
#include "../VKmessageattachment.hpp"

namespace kartun_vkchat
{
        class MessageDataModel: public bb::cascades::DataModel
        {
            Q_OBJECT
        public:
            typedef enum{
                _type_message    = 1,
                _type_attachment = 2,
            }insertItemType;
            MessageDataModel(QMap<uint, Vkmessage*> &inContainer, QObject* parent = 0);
            MessageDataModel(QObject* parent = 0);
            virtual ~MessageDataModel() { }
            // Required interface implementation
            virtual int childCount(const QVariantList& indexPath);
            virtual bool hasChildren(const QVariantList& indexPath);
            virtual QVariant data(const QVariantList& indexPath);
            virtual QString itemType(const QVariantList& indexPath);
            // Other methods
            void insertItem( Vkmessage* &newMessage );
            bool isEmpty();
            void clear();
        public Q_SLOTS:
            void onReadStateChanged(Vkmessage::ReadState newReadState);
            void onUrlChanged(QString newUrl);
        private:
            QHash<int, uint>* m_indexPathCache;
            QMap<uint, Vkmessage*>* m_items;    // Actual items
        };

    } /* namespace vkChatTest */

#endif /* MESSAGEDATAMODEL_H_ */
