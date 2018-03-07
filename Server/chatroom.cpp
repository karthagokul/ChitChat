#include "chatroom.h"
#include "chatconnection.h"
#include "messagehandler.h"


ChatRoom::ChatRoom(QObject *aParent)
    :QObject(aParent)
{
    qRegisterMetaType< Message >("Message");
    qRegisterMetaType< qintptr >("qintptr");
}

void ChatRoom::onLogon(const Message &aMessage,const qintptr&sessionId)
{
    Message onlinelist(Message::Online,QString(),aMessage.sender()+QString("Logged in!"),getBuddies(sessionId));
    broadcastMessage(onlinelist,-1);
}

void  ChatRoom::broadcastMessage(const Message &aMessage,const qintptr&sessionId)
{
    QMapIterator<qintptr,ChatConnection*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        if(i.key()!=sessionId)
        {
            i.value()->write(aMessage.toByteArray());
        }
    }
}

void ChatRoom::onMessageRequest(const Message &aMessage,const qintptr&sessionId)
{
    broadcastMessage(aMessage,-1);
}

QStringList ChatRoom::getBuddies(const qintptr&sessionId)
{
    QStringList buddies;
    QMapIterator<qintptr,ChatConnection*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        //We do not need to send own id to him
        //disabled for development
        //if(i.key()!=socketSessionId)
        {
            buddies<<i.value()->name();
        }
    }
    return buddies;
}

void ChatRoom::closeAllSessions()
{

}

void ChatRoom::onClientDisConnection(const qintptr&sessionId)
{
    //Lets Remove the Online Client from Chatroom
    QString signOffUser;
    QMapIterator<qintptr,ChatConnection*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        if(i.key()==sessionId)
        {
            signOffUser=i.value()->name();
            mOnlineClients.remove(i.key());
        }
    }

    Message logOffMessage(Message::LogOff,QString(),signOffUser+QString(" Left the Room"),\
                          getBuddies(sessionId));
    broadcastMessage(logOffMessage);
}

void ChatRoom::createNewSession(const qintptr socketSessionId)
{
    // Every new connection will be run in a newly created thread
    ChatConnection *newClient = new ChatConnection(socketSessionId, this);
    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(newClient, SIGNAL(finished()), newClient, SLOT(deleteLater()));
    connect(newClient,SIGNAL(disconnecting(qintptr)),this,SLOT(onClientDisConnection(qintptr)));
    connect(newClient,SIGNAL(loggedin(Message,qintptr)),this,SLOT(onLogon(Message,qintptr)));
    connect(newClient,SIGNAL(newmessage(Message,qintptr)),this,SLOT(onMessageRequest(Message,qintptr)));
    //Lets Insert to Online map
    mOnlineClients.insert(socketSessionId,newClient);
    newClient->start();
}
