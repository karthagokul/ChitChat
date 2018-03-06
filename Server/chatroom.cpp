#include "chatroom.h"
#include "chatconnection.h"

ChatRoom::ChatRoom(QObject *aParent)
    :QObject(aParent)
{

}

void ChatRoom::closeAllSessions()
{

}

void ChatRoom::onClientDisConnection(const qintptr&sessionId)
{
    //qDebug()<<"total clients"<<mOnlineClients.count();
    qDebug()<<sessionId<<" left Chatroom";
    //Lets Remove the Online Client from Chatroom
    QMapIterator<qintptr,ChatConnection*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        if(i.key()==sessionId)
        {
            qDebug()<<"Found one";
            mOnlineClients.remove(i.key());
        }
    }
    //qDebug()<<"total clients"<<mOnlineClients.count();
}

void ChatRoom::createNewSession(qintptr socketSessionId)
{
    // Every new connection will be run in a newly created thread
    ChatConnection *newClient = new ChatConnection(socketSessionId, this);
    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(newClient, SIGNAL(finished()), newClient, SLOT(deleteLater()));
    connect(newClient,SIGNAL(disconnecting(qintptr)),this,SLOT(onClientDisConnection(qintptr)));

    //Lets Insert to Online map
    mOnlineClients.insert(socketSessionId,newClient);
    newClient->start();

}
