#include "chatconnection.h"
#include "messagehandler.h"
#include "chatroom.h"
#include <QtNetwork>

ChatConnection::ChatConnection(qintptr aSocketID, QObject *aParent) :
    QThread(aParent)
{
    this->mSocketDescriptor = aSocketID;
}

ChatConnection:: ~ChatConnection()
{
    //    qDebug()<<"Closing Client";
}

void ChatConnection::run()
{
    mSocket = new QTcpSocket();
    // set the ID
    if(!mSocket->setSocketDescriptor(this->mSocketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(mSocket->error());
        return;
    }

    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    //  qDebug() << mSocketDescriptor << " Client connected";

    exec();
}

void ChatConnection::readyRead()
{
    Message newMessage(mSocket->readAll());
    switch (newMessage.type()) {
    case Message::Invalid:
        qDebug()<<"Unsupported Message Type";
        break;
    case Message::LogOn:
        qDebug()<<"New Login : " <<newMessage.message();
        mName=newMessage.sender();
        emit loggedin(newMessage,mSocketDescriptor);
        break;
    case Message::Chat:
        emit newmessage(newMessage,mSocketDescriptor);
        break;
    default:
        break;
    }
}

void ChatConnection::write(QByteArray aData)
{
    mSocket->write(aData);
    mSocket->waitForBytesWritten();
}

void ChatConnection::disconnected()
{
    emit disconnecting(this->mSocketDescriptor);
    mSocket->deleteLater();
    exit(0);
}

