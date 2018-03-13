#include "chatroom.h"
#include "chatsession.h"
#include "messagehandler.h"


ChatRoom::ChatRoom(QObject *aParent)
    :QObject(aParent)
{
    qRegisterMetaType< Message >("Message");
    qRegisterMetaType< qintptr >("qintptr");
}

void ChatRoom::onLogon(const Message &aMessage,const QString&sessionId)
{
    Message onlinelist(Message::Online,QString(),aMessage.sender()+QString(" Logged in!"),getBuddies(sessionId));
    broadcastMessage(onlinelist,QString());
}

void  ChatRoom::broadcastMessage(const Message &aMessage,const QString&sessionId)
{
    QMapIterator<QString,ChatSession*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        if(i.key()!=sessionId)
        {
            i.value()->write(aMessage.toByteArray());
        }
    }
}

void ChatRoom::onMessageRequest(const Message &aMessage,const QString&sessionId)
{
    broadcastMessage(aMessage,QString());
}

QStringList ChatRoom::getBuddies(const QString&sessionId)
{
    QStringList buddies;
    QMapIterator<QString,ChatSession*> i(mOnlineClients);
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

void ChatRoom::onClientDisConnection(const QString&sessionId)
{
    //Lets Remove the Online Client from Chatroom
    QString signOffUser;
    QMapIterator<QString,ChatSession*> i(mOnlineClients);
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

void ChatRoom::registerSession(ChatSession *session)
{
    ChatSession *newClient = session;
    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(newClient, SIGNAL(finished()), newClient, SLOT(deleteLater()));
    connect(newClient,SIGNAL(disconnecting(QString)),this,SLOT(onClientDisConnection(QString)));
    connect(newClient,SIGNAL(loggedin(Message,QString)),this,SLOT(onLogon(Message,QString)));
    connect(newClient,SIGNAL(newmessage(Message,QString)),this,SLOT(onMessageRequest(Message,QString)));
    //Lets Insert to Online map
    mOnlineClients.insert(newClient->id(),newClient);
    qDebug()<<"Total Clients Online"<<mOnlineClients.count();
    newClient->start();
}
