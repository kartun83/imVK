/*
 * MyApplication.cpp
 *
 *  Created on: Jan 28, 2016
 *      Author: kartun
 */

#include <src/MyApplication.h>

namespace kartun_vkchat
{

    MyApplication::MyApplication(int& argc, char** argv): bb::Application(argc, argv)
    {
        // TODO Auto-generated constructor stub

    }

    bool MyApplication::notify(QObject* receiver, QEvent* event)
    {
        bool done = true;
        try {
            done = bb::Application::notify(receiver, event);
//            qDebug() << event->type();
        } catch (const std::exception& ex) {
            // ログや何らかの回復処理
//            qDebug() << "Exception occurred:" << ex.what();
        } catch (...) {
            // ログや何らかの回復処理
//            qDebug() << "BAD Exception occurred";
        }
        return done;
    }

    MyApplication::~MyApplication()
    {
        // TODO Auto-generated destructor stub
    }

} /* namespace kartun_vkchat */
