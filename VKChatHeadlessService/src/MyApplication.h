/*
 * MyApplication.h
 *
 *  Created on: Jan 28, 2016
 *      Author: kartun
 */

#ifndef MYAPPLICATION_H_
#define MYAPPLICATION_H_

#include <bb/Application>

namespace kartun_vkchat
{

    class MyApplication: public bb::Application
    {
    public:
        MyApplication(int& argc, char** argv);
        bool notify(QObject* receiver, QEvent* event);
        virtual ~MyApplication();
    };

} /* namespace kartun_vkchat */

#endif /* MYAPPLICATION_H_ */
