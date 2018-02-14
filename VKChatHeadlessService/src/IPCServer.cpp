/*
 * IPCServer.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: kartun
 */

#include <src/IPCServer.hpp>
#include <QDebug>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

namespace kartun_vkchat
{

    IPCServer::IPCServer(QObject *parent) : QObject(parent), m_port(5483)
    {
        qDebug() << "Starting IPC server";
        m_server = new QTcpServer(this);
        // Connect into the signal/slot mechanism to invoke this class method when a new connection
        // is available
        bool ok = connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
        listen();
    }

    IPCServer::~IPCServer()
    {
        if (m_socket) {
            disconnected();
            m_socket->deleteLater();
        }
        m_server->close();
        m_server->deleteLater();
    }

    void IPCServer::listen()
    {
        qDebug() << "imVK TCP server: start listening for connections";
        m_server->listen(QHostAddress::LocalHost, m_port);
    }

    void IPCServer::newConnection()
    {
        m_socket = m_server->nextPendingConnection();
        if (m_socket->state() == QTcpSocket::ConnectedState) {
            qDebug() << "imVK headless: New connection established.";
        }
        else
        {
            qDebug() << "imVK headless: Connection isn't established.";
        }
        // Make connections for receiving disconnect and read ready signals for the
        // new connection socket
        bool ok = connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        Q_ASSERT(ok);
        ok = connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
    }

    void IPCServer::readyRead()
    {
    }

    void IPCServer::write(const QByteArray& lpsJson)
    {
        if (m_socket && m_socket->state() == QTcpSocket::ConnectedState) {
            qDebug() << "imVK headless: Writing data to socket";
            m_socket->write(lpsJson);
            m_socket->flush();
        }
        else
        {
            qDebug() << "imVK headless:: No sockets connected. Can't write";
        }
    }

    void IPCServer::disconnected()
    {
        qDebug() << "imVK headless: disconnecting...";
        disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        m_socket->close();
    }

} /* namespace kartun_vkchat */
