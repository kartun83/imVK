/*
 * IPCServer.hpp
 *
 *  Created on: Jan 17, 2016
 *      Author: kartun
 */

#ifndef IPCSERVER_HPP_
#define IPCSERVER_HPP_

#include <QObject>

class QTcpServer;
class QTcpSocket;

namespace kartun_vkchat
{

    class IPCServer: public QObject
    {
        Q_OBJECT
    public:
        IPCServer(QObject *parent = 0);
        virtual ~IPCServer();
        /**
             * Method to start listening on the server socket
             * for new connections.
             */
            void listen();

        public Q_SLOTS:
            /**
             * This method is invoked through the signal/slot
             * mechanism when a new connection occurs on the server socket.
             */
            void newConnection();

            /**
             * This method is invoked through the signal/slot mechanism
             * as well, when the socket is ready to read data.
             */
            void readyRead();

            /**
             * This method is used in order to write data out to the socket.
             */
            void write(const QByteArray& lpsJson);

            /**
             * This method is invoked when the socket connection disconnects.
             */
            void disconnected();
        private:
            // The port used for the server socket
            int m_port;
            //The server socket variable
            QTcpServer *m_server;
            //The socket that is created when a connection with the client is made
            QTcpSocket *m_socket;
    };

} /* namespace kartun_vkchat */

#endif /* IPCSERVER_HPP_ */
