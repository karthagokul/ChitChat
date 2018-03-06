#include "chatconnection.h"
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
    // get the information
    QByteArray dataBytes = mSocket->readAll();

    QString data=QString::fromStdString(dataBytes.toStdString());
    qDebug() << mSocketDescriptor << " Client Says: " << data;

     //mSocket->write(Data);
}

void ChatConnection::disconnected()
{
    emit disconnecting(this->mSocketDescriptor);
    qDebug() << mSocketDescriptor << " Disconnected";
    mSocket->deleteLater();
    exit(0);
}
