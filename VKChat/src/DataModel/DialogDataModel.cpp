/*
 * DialogDataModel.cpp
 *
 *  Created on: Dec 6, 2015
 *      Author: kartun
 */

#include <src/DataModel/DialogDataModel.hpp>

namespace kartun_vkchat
{

    DialogDataModel::DialogDataModel(QObject* parent) : bb::cascades::GroupDataModel(parent)
    {
        // TODO Auto-generated constructor stub
        m_indexPathCache.reserve(25);
    }

    void DialogDataModel::insert(QObject *object)
    {
        qDebug() << "Inserting new object in dialogs model";
        GroupDataModel::insert(object);
        // Get index path for object just inserted
        QVariantList indexPath = findExact(object);
        rebuildCache();
    }

    void DialogDataModel::updateByUserID(uint userID)
    {
        qDebug() << "Updating Dialogs model by userID:" << userID;
        if (m_indexPathCache.count(userID))
        {
            qDebug() << "Found in cache" << m_indexPathCache.value(userID);
            emit itemUpdated(m_indexPathCache.value(userID));
        }
    }

    void DialogDataModel::replaceMessage(uint userID, Vkmessage*& newMessage)
    {
        qDebug() << "Replacing message in dialogs model:" << userID << newMessage->getFromID() << " " << newMessage->getUserID();
        if (m_indexPathCache.count(userID))
        {
            this->updateItem(m_indexPathCache.value(userID), newMessage);
        }
    }

    void DialogDataModel::rebuildCache()
    {
        // Rebuild cache
        qDebug() << "Rebuilding dialogs list cache:";
        for ( QVariantList indexPath = this->first(); !indexPath.isEmpty();
             indexPath = this->after(indexPath) )
        {
            Vkmessage* actualItem = qobject_cast<Vkmessage *>(qvariant_cast<QObject *>(this->data(indexPath)));
            if (actualItem->getOut())
            {
                m_indexPathCache.insert(actualItem->getUserID(), indexPath);
            }
            else
            {
                m_indexPathCache.insert(actualItem->getFromID(), indexPath);
            }
        }
    }

} /* namespace kartun_vkchat */
