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
    qDebug()<<"Closing Client";

}

void ChatConnection::run()
{
    // thread starts here
    qDebug() << " Thread started";

    mSocket = new QTcpSocket();

    // set the ID
    if(!mSocket->setSocketDescriptor(this->mSocketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(mSocket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.

    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which
    qDebug() << mSocketDescriptor << " Client connected";

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();
}

void ChatConnection::readyRead()
{

    MessageHandler messageHandle;
    Message newMessage=messageHandle.parseMessageFromClient(mSocket->readAll());
    switch (newMessage.type) {
    case Invalid:
        qDebug()<<"Unsupported Message Type";
        break;
    case LogOn:
        qDebug()<<"New Login : " <<newMessage.message;
        mName=newMessage.message;
        emit loggedin(newMessage,mSocketDescriptor);
        break;
    case Chat:
        emit newmessage(newMessage,mSocketDescriptor);
        break;
    default:

        break;
    }
}

void ChatConnection::write(QByteArray aData)
{
    //Bug if you are calling this function two times immediately, todo
    qDebug()<<"Sending";
    mSocket->write(aData);
    mSocket->waitForBytesWritten();
}


void ChatConnection::disconnected()
{
    emit disconnecting(this->mSocketDescriptor);
    qDebug() << mName << ":Disconnected";
    mSocket->deleteLater();
    exit(0);
}

