/*
 * DialogDataModel.hpp
 *
 *  Created on: Dec 6, 2015
 *      Author: kartun
 */

#ifndef DIALOGDATAMODEL_HPP_
#define DIALOGDATAMODEL_HPP_
#include <bb/cascades/GroupDataModel>
#include "../vkmessage.hpp"

namespace kartun_vkchat
{

    class DialogDataModel: public bb::cascades::GroupDataModel
    {
        Q_OBJECT
    public:
        DialogDataModel(QObject* parent = 0);
        void insert(QObject *object);
        // This one called when we retreived additional info about user. No actual changes to model
        void updateByUserID(uint userID);
        void replaceMessage(uint userID, Vkmessage* &newMessage);
        virtual ~DialogDataModel()
        {
            // TODO Auto-generated destructor stub
        }
    private:
        // userID, indexpath.
        QHash<uint, QVariantList> m_indexPathCache;
        void rebuildCache();
    };

} /* namespace kartun_vkchat */

#endif /* DIALOGDATAMODEL_HPP_ */
